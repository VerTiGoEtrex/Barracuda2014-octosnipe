/*
 * GameTreeState.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: ncrocker
 */
#include "GameTreeState.h"

using namespace std;

GameTreeState::GameTreeState() {
	turnsLeft = 99;
	turn = Owner::WHITE;
	tokens[0] = 1;
	tokens[1] = 1;

	for (int i = 0; i < state.locations.size(); ++i) {
		state.locations[i] = Owner::UNOWNED;
	}
}

GameTreeState::GameTreeState(game_state &gameState) {
	turnsLeft = gameState.moves_remaining;
	turn = Owner::WHITE;
	tokens[Owner::WHITE] = gameState.your_tokens;
	tokens[Owner::BLACK] = gameState.their_tokens;

	//Setup the board
	for (int x = 0; x < gameState.board.size(); ++x) {
		for (int y = 0; y < gameState.board[x].size(); ++y) {
			for (int z = 0; z < gameState.board[x][y].size(); ++z) {
				if (gameState.board[x][y][z] == 0) {
					state.owner(x, y, z) = Owner::UNOWNED;
				} else if (gameState.board[x][y][z] == gameState.player_number) {
					state.owner(x, y, z) = Owner::WHITE;
				} else {
					state.owner(x, y, z) = Owner::BLACK;
				}
			}
		}
	}
}

GameTreeState::GameTreeState(const GameTreeState &original) :
						turn(original.turn), state(original.state), turnsLeft(original.turnsLeft) {
	copy(original.tokens, original.tokens + 2, tokens);
}

int GameTreeState::getHeuristicValue() {
	int dim = state.getDim();
	int validBallLen = state.locations.size();

	int h = 0;
	for (int z = 0; z < dim; ++z) {
		for (int y = 0; y < dim - z; ++y) {
			for (int x = 0; x < dim - y - z; ++x) {
				auto &owner = state.owner(x, y, z);
				//Handle strange end-game case
				if (z > 0 && owner == Owner::UNOWNED) {
					if (state.owner(x, y, z-1) == Owner::WHITE
							&& state.owner(x+1, y, z-1) == Owner::WHITE
							&& state.owner(x, y+1, z-1) == Owner::WHITE) {
						owner = Owner::WHITE;
					} else if (state.owner(x, y, z-1) == Owner::BLACK
							&& state.owner(x+1, y, z-1) == Owner::BLACK
							&& state.owner(x, y+1, z-1) == Owner::BLACK) {
						owner = Owner::BLACK;
					}
				}
			}
		}
	}

	for (int i = 0; i < 220; ++i) {
		if (state.locations[i] == Owner::WHITE) {
			++h;
		} else if (state.locations[i] == Owner::BLACK) {
			--h;
		}
	}

	if (gameOver()) {
		if (h > 0)
			return 10000;
		else if (h == 0)
			return 0;
		else
			return -10000;
	}

	return h;
}

void GameTreeState::applyMove(Move &m) {
	if (!m.claim) {
		// Wait
		++tokens[turn];
	} else {
		// Claim ball and children
		for (int z = 0; z <= m.z; ++z) {
			for (int y = m.y; y <= m.y + m.z - z; ++y) {
				for (int x = m.x; x <= m.x + m.y - y + m.z - z; ++x) {
					Owner& ballOwner = state.owner(x, y, z);
					assert(ballOwner == turn || ballOwner == Owner::UNOWNED);
					ballOwner = turn;
				}
			}
		}

		// Deduct tokens
		tokens[turn] -= m.z + 1;
		assert(tokens[turn] >= 0);
	}

	// Flip turn
	if (turn == Owner::WHITE)
		turn = Owner::BLACK;
	else
		turn = Owner::WHITE;

	//Deduct turns
	turnsLeft--;
}

std::vector<Move> GameTreeState::getMoves() {
	vector<Move> moves;

	// Wait is trivial
	moves.push_back(Move { });

	// Figure out what we can claim
	int dim = state.getDim();
	int validBallLen = state.locations.size();
	vector<bool> validBall(validBallLen, false);

	// Fill in the table
	for (int z = 0; z < min(dim, tokens[turn]); ++z) {
		for (int y = 0; y < dim - z; ++y) {
			for (int x = 0; x < dim - y - z; ++x) {
				Owner &owner = state.owner(x, y, z);
				//Check to see if we can claim this ball)
				if (z == 0) {
					//Z = 0 is easy, just check if ball is unclaimed
					if (owner == Owner::UNOWNED) {
						//Square can be claimed
						moves.push_back(Move(x, y, z));
					}
					validBall[state.getCoord(x, y, z)] = (owner == turn
							|| owner == Owner::UNOWNED);
				} else {
					// Z > 0 is harder, check if the balls under us are unclaimed OR owned by us
					// and this call is unclaimed
					if (validBall[state.getCoord(x, y, z - 1)]
								  && validBall[state.getCoord(x + 1, y, z - 1)]
											   && validBall[state.getCoord(x, y + 1, z - 1)]) {
						if (owner == Owner::UNOWNED) {
							moves.push_back(Move(x, y, z));
						}
						validBall[state.getCoord(x, y, z)] = (owner == turn
								|| owner == Owner::UNOWNED);
					} else {
						validBall[state.getCoord(x, y, z)] = false;
					}
				}
			}
		}
	}
	return moves;
}

Owner GameTreeState::getTurn(){
	return turn;
}

bool GameTreeState::gameOver(){
	if (!turnsLeft) {
		return true;
	}

	int dim = state.getDim();
	int h = 0;
	int unfilledBot = ((dim+1) * dim) / 2;
	for (int i = 0; i < unfilledBot; ++i) {
		if (state.locations[i] == Owner::UNOWNED) {
			return false;
		}
	}
	return true;
}

