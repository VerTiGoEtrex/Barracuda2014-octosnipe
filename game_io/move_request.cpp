#include "move_request.h"

move_request::move_request(Json::Value root) {
    this->set_move_request(root);
}

void move_request::set_move_request(Json::Value root) {
    this->state = new game_state(root);
}

move_request::~move_request() {
    delete this->state;
}
