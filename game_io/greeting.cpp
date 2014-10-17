#include "greeting.h"

greeting::greeting(Json::Value root) {
    this->set_greeting(root);
}

void greeting::set_greeting(Json::Value root) {
    this->team_id = root["team_id"].asInt();
}
