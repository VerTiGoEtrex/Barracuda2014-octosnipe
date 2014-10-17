#include "json_response.h"

void json_response::set_type(string type) {
    this->json_root["type"] = "move";
}

void json_response::set_request_id(int req_id) {
    this->json_root["id"] = req_id;
}

move_response::move_response() {
    this->set_type("move");
}
