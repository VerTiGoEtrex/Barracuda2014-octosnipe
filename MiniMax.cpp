#include "MiniMax.h"

#include <utility>
#include <cassert>
#include <limits>

using namespace std;

int minimax(GameTreeState& state, int remainingDepth, int alpha, int beta);

pair<int, Move> getBestMove(GameTreeState& state, int maxDepth) {
	assert (state.getTurn() == Owner::WHITE);

	int alpha = numeric_limits<int>::min();
	int beta = numeric_limits<int>::max();

	Move bestMove;

	// Iterate over all of our moves and pick the best one.
	// Alpha represents the best so far that we (white) can guarantee.
	// Beta represents the best they can do.
	for (Move m: state.getMoves()) {
		GameTreeState newState {state};
		newState.applyMove(m);
		int score = minimax(state, maxDepth - 1, alpha, beta);
		if (score > alpha) {
			alpha = score;
			bestMove = m;
		}
	}

	return make_pair(alpha, bestMove);
}

int minimax(GameTreeState& state, int remainingDepth, int alpha, int beta) {
	assert (remainingDepth >= 0);

	if (remainingDepth == 0 || state.gameOver())
		return state.getHeuristicValue();

	if (state.getTurn() == Owner::WHITE) {
		// Maximize
		int bestSoFar = numeric_limits<int>::min();

		for (Move m: state.getMoves()) {
			GameTreeState newState {state};
			newState.applyMove(m);
			int newValue = minimax(newState, remainingDepth - 1, alpha, beta);

			if (beta < newValue)
				// We know black can do better than this, so they certainly won't take this tree.
				// No need to keep checking.
				return newValue;

			bestSoFar = max(bestSoFar, newValue);
			alpha = max(alpha, newValue);
		}
		return bestSoFar;
	}
	else {
		// Minimize
		int worstSoFar = numeric_limits<int>::max();

		for (Move m: state.getMoves()) {
			GameTreeState newState {state};
			newState.applyMove(m);
			int newValue = minimax(newState, remainingDepth - 1, alpha, beta);

			if (alpha > newValue)
				// We know that we (white) can beat this. We won't touch it.
				return newValue;

			worstSoFar = min(worstSoFar, newValue);
			beta = min(worstSoFar, newValue);
		}
		return worstSoFar;
	}
}
