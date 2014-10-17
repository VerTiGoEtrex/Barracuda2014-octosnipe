#pragma once

#include "json/json.h"

class greeting {
    public:
        greeting(Json::Value);
        void set_greeting(Json::Value);
        int team_id;
};
