#pragma once

#include <string>
#include "json/json.h"

#include "../json_socket/json_socket.h"
#include "error_msg.h"
#include "move_request.h"
#include "game_result.h"
#include "greeting.h"
#include "json_response.h"
#include "move_result.h"
#include "take_space_response.h"
#include "wait_response.h"

using namespace std;

class game_player {
    public:
        virtual void error(error_msg*) = 0;
        virtual move_response* move(move_request*) = 0;
        virtual void server_greeting(greeting*) = 0;
        virtual void game_over(game_result*) = 0;
        virtual void hand_done(move_result*) = 0;
        virtual void move_received(move_result*) = 0;
};

class game_mediator {
    public:
       game_mediator(game_player*, json_socket*);
       void start();
       game_player* player;
       json_socket* jsock;
       int player_number;
};
