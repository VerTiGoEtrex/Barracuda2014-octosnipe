#pragma once

#include "json_response.h"
#include "game_state.h"

class wait_response : public move_response {
    public:
        wait_response();
};
