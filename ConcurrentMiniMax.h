#ifndef CONCURRENTMINIMAX_H
#define CONCURRENTMINIMAX_H

#include "GameTreeState.h"
#include <utility>


std::pair<int, Move> getBestMoveConcurrent(GameTreeState& state, int maxDepth);




#endif
