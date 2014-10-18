#include "ConcurrentMiniMax.h"
#include "MiniMax.h"
#include "GameTreeState.h"

#include <queue>
#include <utility>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <limits>

using namespace std;

struct QueueItem {
	QueueItem(GameTreeState& state) : state(state) {}
	QueueItem(const QueueItem& other) : m(other.m), state(other.state), f(other.f) {}
	Move m;
	GameTreeState state;
	function<void (GameTreeState&, Move)> f;
};

pair<int, Move> getBestMoveConcurrent(GameTreeState& state, int maxDepth) {
	// Queue of minimaxes that need to be run
	queue<QueueItem> miniMaxes;
	mutex queueLock;

	int alpha = numeric_limits<int>::min();
	Move bestMove;
	mutex alphaLock;

	auto worker = [&miniMaxes, &queueLock]() -> void {
		// Get a minimax item if there is one
		queueLock.lock();

		while (true) {
			if (miniMaxes.empty()) {
				break;
			}
			auto miniMaxInstance = miniMaxes.front();
			miniMaxes.pop();
			queueLock.unlock();

			// Run it
			miniMaxInstance.f(miniMaxInstance.state, miniMaxInstance.m);

			queueLock.lock();
		}

		queueLock.unlock();
	};

	auto runMiniMax = [maxDepth, &alpha, &bestMove, &alphaLock](GameTreeState& state, Move m) {
		alphaLock.lock();
		int currentAlpha = alpha;
		alphaLock.unlock();

		int result = minimax(state, maxDepth - 1, currentAlpha, numeric_limits<int>::max());

		alphaLock.lock();
		if (result > alpha) {
			alpha = result;
			bestMove = m;
		}
		alphaLock.unlock();
	};

	queueLock.lock();
	auto moves = state.getMoves();
	for (auto it = moves.rbegin(); it != moves.rend(); ++it) {
		QueueItem item{state};
		item.state.applyMove(*it);
		item.m = *it;
		item.f = runMiniMax;
		miniMaxes.push(item);
	}
	queueLock.unlock();

	vector<thread> workerThreads;
	for (int i = 0; i < thread::hardware_concurrency(); ++i)
		workerThreads.push_back(thread(worker));

	for (auto& t: workerThreads)
		t.join();


	alphaLock.lock();
	int ret = alpha;
	Move retMove = bestMove;
	alphaLock.unlock();

	return make_pair(ret, retMove);
}
