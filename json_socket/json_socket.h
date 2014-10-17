#pragma once

#include "json/json.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class json_socket {
    public:
        json_socket(string, string);
        ~json_socket();
        virtual bool send_msg(Json::Value);
        virtual Json::Value recv_msg();
        int bsock_fd;
        string last_jsonstr;
};

struct UnableToConnect : exception {};
struct NotParseableJson : exception {};
struct ConnectionError : exception {};
struct PeerDisconnected : exception{};
