#include "error_msg.h"

error_msg::error_msg(Json::Value root) {
    this->set_error_msg(root);
}

void error_msg::set_error_msg(Json::Value err) {
    this->seen_host = err["state"]["seen_host"].asString();
    this->message = err["state"]["error"].asString();
}
