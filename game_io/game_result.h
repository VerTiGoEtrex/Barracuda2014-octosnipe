#pragma once

#include "json/json.h"

#include "game_state.h"

class game_result {
    public:
        game_result(Json::Value);
        ~game_result();
        void set_game_result(Json::Value);

        game_state *state;
        unsigned int winner;

        friend ostream& operator<<(ostream &os, const game_result &gr);
};
