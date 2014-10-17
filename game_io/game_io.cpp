#include "game_io.h"
#include <iostream>

game_mediator::game_mediator(game_player* player, json_socket* jsock) {
    this->player = player;
    this->jsock = jsock;
}

void game_mediator::start() {
    json_response* resp;
    string type;
    Json::Value root;

    while(1) {
        resp = NULL;
        root = this->jsock->recv_msg();
        type = root["type"].asString();

        if (type == "request") {
            move_request *req = new move_request(root);
            this->player_number = req->state->player_number;
            resp = player->move(req);
            delete req;
        }
        else if (type == "greetings_program") {
            greeting *greet = new greeting(root);
            player->server_greeting(greet);
            delete greet;
        }
        else if (type == "error") {
            error_msg *err = new error_msg(root);
            player->error(err);
            delete err;
        }
        else if (type == "game_over") {
            game_result *res = new game_result(root);
            player->game_over(res);
            delete res;
        }
        else if (type == "move_result") {
            if (!root["state"]["error"].isNull() && !root["state"]["error"].asString().empty())
                cout << "Error performing move: " << root["state"]["error"] << endl;
            else {
                move_result* res = new move_result(root);
                player->move_received(res);
                delete res;
            }
        }
        else {
            cout << "Unknown type: " << type << " seen" << endl;
            cout << this->jsock->last_jsonstr << endl;
        }

        if (resp != NULL) {
            resp->set_request_id(root["id"].asInt());
            this->jsock->send_msg(resp->json_root);
            delete resp;
        }
    }
}
