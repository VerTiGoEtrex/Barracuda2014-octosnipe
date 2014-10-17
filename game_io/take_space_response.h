#pragma once

#include "json_response.h"
#include "game_state.h"

class take_space_response : public move_response {
    public:
        take_space_response(board_point);
    private:
        board_point point;
};
