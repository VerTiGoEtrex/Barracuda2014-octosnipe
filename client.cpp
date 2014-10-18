#ifndef _MSC_VER
#include <unistd.h>
#define SLEEP(x) sleep(x)
#else
#include <windows.h>
#define SLEEP(x) Sleep(x)
#endif

#include <thread>

#include "client.h"
#include "json_socket/json_socket.h"
#include "MiniMax.h"
#include "ConcurrentMiniMax.h"
#include "GameTreeState.h"
#include <chrono>

std::atomic<bool> haltarino;

#define OD "==============================\n"

void client::error(error_msg* err) {
	cout << "error: " << err->message << endl;
	throw GameError();
}

void runnerFn(GameTreeState &s, move_response *&r, int turnsLeft) {
	int i = 2;
	for (int i = 2; i <= turnsLeft; ++i) {
		auto res = getBestMoveConcurrent(s, i);
		if (haltarino)
			return;

		if (r != NULL) {
			delete r;
		}
		if (!res.second.claim) {
			r = new wait_response();
		} else {
			board_point p;
			p.x = res.second.x;
			p.y = res.second.y;
			p.z = res.second.z;
			r = new take_space_response(p);
		}
		cout << "Current depth: " << i << endl;
	}
	haltarino.store(true);
}

move_response* client::move(move_request* req) {
	haltarino.store(false);
	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

	cout << OD;
	cout << *req << endl;
	cout << OD;

	GameTreeState currentState(*req->state);

	move_response* r;
	thread runner(runnerFn, std::ref(currentState), std::ref(r), req->state->moves_remaining + 1);
	chrono::nanoseconds time_span;
	do {
		chrono::milliseconds dura(5);
		this_thread::sleep_for(dura);

		chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
		time_span = chrono::duration_cast<chrono::nanoseconds>(t2 - t1);
	} while (!haltarino.load() && time_span.count() < min(req->state->time_remaining_ns, (unsigned long long) 100 * 1000000));
	haltarino.store(true);
	cout << "Joining" << endl;
    runner.join();
    cout << "Joined. Response " << (r ? "is not" : "is") <<  " null." << endl;
    //cout << "R: " << r->json_root["type"] << endl;
	return r;

	// LE
	// BARRACUDA
	// CODE
	/*
	 * if (random_wait(random_generator))
		return new wait_response();
	else {
		uniform_int_distribution<int> random_space(0, req->state->legal_moves.size() - 1);
		return new take_space_response(req->state->legal_moves[random_space(random_generator)]);
	}
	 */
}

void client::server_greeting(greeting* greet) {
	cout << OD;
	cout << "Connected to server." << endl;
	cout << "Team: " << greet->team_id << endl;
	cout << OD;
}

void client::game_over(game_result* r) {
	cout << OD;
	cout << "*** GAME OVER ***" << endl;
	cout << *r << endl;
	cout << OD;
}

void client::hand_done(move_result* r) {
	cout << OD;
	cout << "*** HAND DONE ***" << endl;
	cout << *r << endl;
	cout << OD;
}

void client::move_received(move_result* r) {
	cout << OD;
	cout << "*** MOVE RECV ***" << endl;
	//cout << *r << endl;
	cout << OD;
}

int main(int argc, char * argv[]) {
#ifdef _MSC_VER
	// Initialize Winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

	string server = "cuda.contest";
	string port = "9999";

	switch (argc) {
	case 3:
		port = argv[2];
	case 2:
		server = argv[1];
	case 1:
		break;
	default:
		cout << "Usage: " << argv[0] << " [host [port]]" << endl;
		return 0;
	}

	int wait = 10;
	for (;;) {
		try {
			json_socket contest_server = json_socket(server, "9999");

			client myclient = client();

			game_mediator game = game_mediator(&myclient, &contest_server);
			game.start();
		}
		catch (UnableToConnect) {
			cout << "Unable to connect. Waiting 10 seconds..." << endl;
			wait = 10;
		}
		catch (NotParseableJson) {
			cout << "Unparsable JSON encountered. Reconnecting..."
					<< endl;
			wait = 1;
		}
		catch (ConnectionError) {
			cout << "Unexpectedly unable to read from socket. Reconnecting..."
					<< endl;
			wait = 1;
		}
		catch (PeerDisconnected) {
			cout << "Server disconnected. Reconnecting..." << endl;
			wait = 1;
		}
		catch (GameError) {
			break;
		}
		SLEEP(wait);
	}

	return 0;
}
