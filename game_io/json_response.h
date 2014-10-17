#pragma once

#include <string>
#include "json/json.h"

using namespace std;

class json_response {
    public:
        virtual void set_type(string);
        virtual void set_request_id(int);
        Json::Value json_root;
};

class move_response : public json_response {
    public:
        move_response();
};
