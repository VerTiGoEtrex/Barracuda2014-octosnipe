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

ostream& operator<<(ostream &os, const move_request &mr) {
	os << "MOVE REQUEST" << endl;
	os << *mr.state << endl;
	return os;
}
