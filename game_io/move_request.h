#pragma once

#include <string>

#include "json/json.h"
#include "game_state.h"

using namespace std;

class move_request {
    public:
        ~move_request();
        move_request(Json::Value);
        void set_move_request(Json::Value);

        game_state *state;

        friend ostream& operator<<(ostream &os, const move_request &mr);
};
