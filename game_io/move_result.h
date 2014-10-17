#pragma once

#include "json/json.h"
#include "game_state.h"

class move_result {
    public:
        ~move_result();
        move_result(Json::Value);
        void set_move_result(Json::Value);

        game_state *state;
        board_point *claimed;
};
