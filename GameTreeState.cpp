/*
 * GameTreeState.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: ncrocker
 */
#include "GameTreeState.h"

using namespace std;

GameTreeState::GameTreeState() :
		turn(Owner::WHITE), state(), tokens( { 0, 0 }) {
}

GameTreeState::GameTreeState(GameTreeState &original) :
		turn(original.turn), state(original.state) {
	copy(original.tokens, original.tokens + 2, tokens);
}

int GameTreeState::getHeuristicValue() {
	int dim = state.getDim();
	int validBallLen = state.getCoord(dim - 1, dim - 1, dim - 1) + 1;

	int h = 0;
	int unfilledBot = ((dim-1) * dim) / 2;
	for (int i = 0; i < validBallLen; ++i) {
		if (i < ((dim-1) * dim) / 2 && state.locations[i] != Owner::UNOWNED) {
			++unfilledBot;
		}
		if (state.locations[i] == Owner::WHITE) {
			++h;
		} else if (state.locations[i] == Owner::BLACK) {
			--h;
		}
	}

	if (!unfilledBot) {
		if (h > 0)
			return 10000;
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
					auto& ballOwner = state.getOwner(x, y, z);
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
}

std::vector<Move> GameTreeState::getMoves() {
	vector<Move> moves;

	// Wait is trivial
	moves.push_back(Move { });

	// Figure out what we can claim
	int dim = state.getDim();
	int validBallLen = state.getCoord(dim - 1, dim - 1, dim - 1) + 1;
	unique_ptr<bool*> validBall(new bool[validBallLen]);

	// Fill in the table
	for (int z = 0; z < min(dim, tokens[turn]); ++z) {
		for (int y = 0; y < dim - z; ++y) {
			for (int x = 0; x < dim - y - z; ++x) {
				Owner &owner = state.getOwner(x, y, z);
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

