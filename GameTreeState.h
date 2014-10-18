#ifndef GAMETREESTATE_H
#define GAMETREESTATE_H

#include <vector>
#include <cassert>
#include <memory>
#include <algorithm>
#include <utility>

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

class Tetrahedron {
public:
	Tetrahedron(int dim) : dim(dim), locationsLen(getCoord(dim-1, dim-1, dim-1) + 1) {
		locations = std::unique_ptr<Owner>(new Owner[locationsLen]);
	}

	Tetrahedron() : dim(0), locationsLen(0), locations() {}

	Tetrahedron(const Tetrahedron& other)
	: dim(other.dim), locationsLen(other.locationsLen) {

		locations = std::unique_ptr<Owner>(new Owner[locationsLen]);
		std::copy(other.locations.get(), other.locations.get() + other.locationsLen, locations.get());
	}
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
	Owner& getOwner(int x, int y, int z) {
		return locations.get()[getCoord(x, y, z)];
	}
	int getCoord(int x, int y, int z) {
		assert ((x+y+z) < dim);

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
	std::unique_ptr<Owner> locations;
private:
	void swap (Tetrahedron& other) {
		std::swap(locations, other.locations);
		std::swap(dim, other.dim);
		std::swap(locationsLen, other.locationsLen);
	}

	int dim;
	int locationsLen;


};

class GameTreeState {
public:
	GameTreeState(game_state &state);
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
