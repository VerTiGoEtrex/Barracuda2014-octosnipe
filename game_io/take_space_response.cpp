#include "take_space_response.h"

take_space_response::take_space_response(board_point point) {
    this->json_root["location"][0] = point.x;
    this->json_root["location"][1] = point.y;
    this->json_root["location"][2] = point.z;
}
