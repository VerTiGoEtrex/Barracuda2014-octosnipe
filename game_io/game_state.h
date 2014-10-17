#pragma once

#include "json/json.h"
#include <iostream>
#include <vector>

using namespace std;

struct board_point {
    unsigned int x, y, z;
};

class game_state {
    public:
        game_state(Json::Value);
        void set_game_state(Json::Value);

        vector<board_point> legal_moves;

        vector< vector< vector<int> > > board;

        int game_id;
        int your_points;
        int their_points;
        int opponent_id;
        int player_number;
        int your_tokens;
        int their_tokens;
        unsigned long long time_remaining_ns;
        int moves_remaining;
};
