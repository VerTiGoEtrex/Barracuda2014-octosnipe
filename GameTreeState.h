#ifndef GAMETREESTATE_H
#define GAMETREESTATE_H

#include <vector>
#include <cassert>
#include <memory>
#include <algorithm>
#include <utility>

enum class Owner {
	WHITE,
	BLACK,
	UNOWNED
};

struct Move {
	bool claim; // Wait when claim == false
	int x, y, z;
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
private:
	void swap (Tetrahedron& other) {
		std::swap(locations, other.locations);
		std::swap(dim, other.dim);
		std::swap(locationsLen, other.locationsLen);
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
	int dim;
	int locationsLen;
	std::unique_ptr<Owner> locations;

};

class GameTreeState {
public:
	GameTreeState(GameTreeState& original);
	int getHeuristicValue();
	std::vector<Move> getMoves();
	void applyMove(Move m);
private:
	Owner turn;

	Tetrahedron state;
};

#endif
