#pragma once

#include "json/json.h"
#include <string>

using namespace std;

class error_msg {
    public:
        error_msg(Json::Value);
        void set_error_msg(Json::Value);
        string seen_host;
        string message;
};
