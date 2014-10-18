#include "game_state.h"

game_state::game_state(Json::Value state) {
    this->set_game_state(state);
}

void game_state::set_game_state(Json::Value message) {
    this->game_id = message["game_id"].asInt();

    Json::Value state = message["state"];
    
    if (!state["legal_moves"].isNull()) {
        this->legal_moves.clear();

        for (unsigned int i = 0; i < state["legal_moves"].size(); i++) {
            if (state["legal_moves"][i].size() == 3) {
                board_point move = {
                    state["legal_moves"][i][0].asUInt(),
                    state["legal_moves"][i][1].asUInt(),
                    state["legal_moves"][i][2].asUInt()
                };
                this->legal_moves.push_back(move);
            }
            else
                cout << "Item in legal_moves had incorrect size: " << state["legal_moves"][i].size() << endl;
        }
    }

    this->board.clear();
    unsigned int size = state["board"].size();

    if (size > 255) {
        cout << "Board parsing error: board size exceeds sane limit" << endl;
        return;
    }

    for (unsigned int x = 0; x < size; x++) {
        if (state["board"][x].size() != size - x) {
            cout << "Board parsing error: expected " << (size - x) << " y rows for x = " << x << ", found " << state["board"][x].size() << endl;
            break;
        }

        vector< vector<int> > x_slice;
        for (unsigned int y = 0; y < (size - x); y++) {
            if (state["board"][x][y].size() != size - x - y) {
                cout << "Board parsing error: expected " << (size - x - y) << " z rows for (x, y) = (" << x << ", " << y << "), found " << state["board"][x][y] << endl;
                break;
            }

            vector<int> y_slice;
            for (unsigned int z = 0; z < (size - x - y); z++) {
                if (!state["board"][x][y][z].isInt()) {
                    cout << "Board parsing error: item at (x, y, z) = (" << x << ", " << y << ", " << z << ") is not an integer" << endl;
                    break;
                }
                y_slice.push_back(state["board"][x][y][z].asInt());
            }
            x_slice.push_back(y_slice);
        }
        this->board.push_back(x_slice);
    }

    if (!state["time_remaining_ns"].isNull())
        this->time_remaining_ns = state["time_remaining_ns"].asUInt64();

    if (!state["moves_remaining"].isNull())
        this->moves_remaining = state["moves_remaining"].asInt();

    this->player_number = state["player"].asInt();

    int player1_score = state["score"]["player1"].asInt();
    int player2_score = state["score"]["player2"].asInt();
    this->your_points = this->player_number == 1 ? player1_score : player2_score;
    this->their_points = this->player_number == 2 ? player1_score : player2_score;

    this->your_tokens = state["tokens"].asInt();
    if (!state["opponent_tokens"].isNull())
        this->their_tokens = state["opponent_tokens"].asInt();
    //this->opponent_id = state["opponent_id"].asInt();
}

ostream& operator<<(ostream &os, const game_state &gs) {
	os << "GAME STATE" << endl;
	os << "Game id: " << gs.game_id << endl;
	os << "Opponent id: " << gs.opponent_id << endl;
	os << "Player number: " << gs.player_number << endl;
	os << "Remaining time: " << gs.time_remaining_ns << endl;
	os << "Number of legal moves: " << gs.legal_moves.size() << endl;
	os << "Our points: " << gs.your_points << endl;
	os << "Our tokens: " << gs.your_tokens << endl;
	os << "Their points: " << gs.their_points << endl;
	os << "Their tokens: " << gs.their_tokens << endl;
	return os;
}

ostream& operator<<(ostream &os, const board_point &bp) {
	os << "BOARD POINT" << endl;
	os << "(" << bp.x << ", " << bp.y << ", " << bp.z << ")" << endl;
	return os;
}
