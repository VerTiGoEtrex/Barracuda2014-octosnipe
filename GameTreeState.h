#ifndef GAMETREESTATE_H
#define GAMETREESTATE_H

#include <vector>
#include <cassert>
#include <memory>
#include <algorithm>
#include <utility>
#include "game_io/game_state.h"

enum Owner {
	WHITE = 0,
	BLACK = 1,
	UNOWNED = 2
};

struct Move {
	bool claim; // Wait when claim == false
	int x, y, z;
	Move() : claim(false) {}
	Move(int x, int y, int z) : claim(true), x(x), y(y), z(z) {}
};

struct Tetrahedron {
	vector<Owner> locations;
	const int dim = 10;

	Tetrahedron() {
		locations.resize(220);
	}

	Tetrahedron(const Tetrahedron& other) {
		locations.resize(220);
		std::copy(other.locations.begin(), other.locations.end(), locations.begin());
	}


	Owner& owner(int x, int y, int z) {
		assert ((x+y+z) < dim);
		return locations[getCoord(x, y, z)];
	}
	int getCoord(int x, int y, int z) {
		int coord = 0;
		int levelDim = dim - z;

		// Handle the z offset
		coord += (dim*(dim+1)*(dim+2))/6 - (levelDim*(levelDim+1)*(levelDim+2))/6;

		// Handle the y offset
		coord += ((levelDim + (levelDim - y + 1)) * y) / 2;

		// Handle the x offset
		coord += x;

		return coord;
	}
	int getDim() {
		return dim;
	}
};

class GameTreeState {
public:
	GameTreeState();
	GameTreeState(game_state &gameState);
	GameTreeState(const GameTreeState& original);
	int getHeuristicValue();
	std::vector<Move> getMoves();
	void applyMove(Move &m);
	Owner getTurn();
	bool gameOver();
private:
	bool canClaim(Owner &owner, std::unique_ptr<bool> &validBall);
	Owner turn;
	int tokens[2];
	Tetrahedron state;
	int turnsLeft;
};

#endif
