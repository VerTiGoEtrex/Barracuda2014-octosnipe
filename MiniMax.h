#ifndef MINIMAX_H
#define MINIMAX_H

#include "GameTreeState.h"
#include <utility>
#include <atomic>

extern atomic<bool> haltarino;

std::pair<int, Move> getBestMove(GameTreeState& state, int maxDepth);
int minimax(GameTreeState& state, int remainingDepth, int alpha, int beta);



#endif
