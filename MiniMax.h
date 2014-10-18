#ifndef MINIMAX_H
#define MINIMAX_H

#include "GameTreeState.h"
#include <utility>

std::pair<int, Move> getBestMove(GameTreeState& state, int maxDepth);




#endif
