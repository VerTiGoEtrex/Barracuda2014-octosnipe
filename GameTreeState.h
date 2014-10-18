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
	std::vector<Owner> locations;
	int dim;

	Tetrahedron(int dim) : dim(dim), locations(getCoord(0, 0, dim-1)) {}
	Tetrahedron() : dim(0), locations() {}

	Tetrahedron(const Tetrahedron& other)
	: dim(other.dim), locations(other.locations) {}

	Tetrahedron (Tetrahedron&& other) {
		swap(other);
	}
	Tetrahedron& operator= (Tetrahedron&& other) {
		swap(other);
		return *this;
	}
	Tetrahedron& operator= (const Tetrahedron& other) {
		Tetrahedron otherCopy(other);
		swap(otherCopy);
		return *this;
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
		coord += ((levelDim + (levelDim - y + 1)) * levelDim) / 2;

		// Handle the x offset
		coord += x;

		return coord;
	}
	int getDim() {
		return dim;
	}
	void swap (Tetrahedron& other) {
		std::swap(locations, other.locations);
		std::swap(dim, other.dim);
	}
};

class GameTreeState {
public:
	GameTreeState(game_state &gameState);
	GameTreeState(GameTreeState& original);
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
