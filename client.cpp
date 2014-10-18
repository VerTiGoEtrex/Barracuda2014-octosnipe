#ifndef _MSC_VER
#include <unistd.h>
#define SLEEP(x) sleep(x)
#else
#include <windows.h>
#define SLEEP(x) Sleep(x)
#endif

#include "client.h"
#include "json_socket/json_socket.h"

#define OD "==============================\n"

void client::error(error_msg* err) {
    cout << "error: " << err->message << endl;
    throw GameError();
}

move_response* client::move(move_request* req) {
	cout << OD;
	cout << *req << endl;
	cout << OD;

	// LE
	// BARRACUDA
	// CODE

    if (random_wait(random_generator))
        return new wait_response();
    else {
        uniform_int_distribution<int> random_space(0, req->state->legal_moves.size() - 1);
        return new take_space_response(req->state->legal_moves[random_space(random_generator)]);
    }

}

void client::server_greeting(greeting* greet) {
	cout << OD;
    cout << "Connected to server." << endl;
    cout << "Team: " << greet->team_id << endl;
    cout << OD;
}

void client::game_over(game_result* r) {
	cout << OD;
    cout << *r << endl;
    cout << OD;
}

void client::hand_done(move_result* r) {
    cout << OD;
    cout << *r << endl;
    cout << OD;
}

void client::move_received(move_result* r) {
	cout << OD;
    cout << *r << endl;
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
