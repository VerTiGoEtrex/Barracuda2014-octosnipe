#include "move_result.h"

move_result::move_result(Json::Value root) : claimed(NULL) {
    this->set_move_result(root);
}

void move_result::set_move_result(Json::Value root) {
    Json::Value claimed_json = root["state"]["claimed"];
    if (!claimed_json.isNull()) {
        this->claimed = new board_point;
        this->claimed->x = claimed_json[0].asInt();
        this->claimed->y = claimed_json[1].asInt();
        this->claimed->z = claimed_json[2].asInt();
    }
    else {
        if (this->claimed)
            delete this->claimed;
        this->claimed = NULL;
    }

    this->state = new game_state(root);
}

move_result::~move_result() {
    delete this->state;
    delete this->claimed;
}
