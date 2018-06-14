#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <boost/optional.hpp>
#include "GameState.h"
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;

int alphabeta(GameState s, unordered_map<string,int>& cache, int depth,
	int alpha, int beta) {
	if (boost::optional<int> check = s.is_game_over()) {
		return *check * 8;
	} else if (depth == 0) {
		return s.height_heuristic();
	}

	string state_str = s.board_to_string();

	if (s.turn == 0) {
		if (boost::optional<int> cached_value = s.get_cached_value(cache)) {
			cout << "Cache Hit!" << endl;
			return *cached_value;
		}

		int best_val = -8;
		vector<GameState> v = s.get_ordered_moves(&GameState::height_heuristic);
		for (GameState next : v) {
			best_val = max(best_val, alphabeta(next, cache, depth - 1, alpha, beta));
			alpha = max(alpha, best_val);
			if (beta <= alpha || best_val == 8) {
				break;
			}
		}
		cache[state_str] = best_val;
		return best_val;
	} else {
		if (boost::optional<int> cached_value = s.get_cached_value(cache)) {
			cout << "Cache Hit!" << endl;
			return -1 * (*cached_value);
		}

		int best_val = 8;
		vector<GameState> v = s.get_ordered_moves(&GameState::height_heuristic);
		for (GameState next : v) {
			best_val = min(best_val, alphabeta(next, cache, depth - 1, alpha, beta));
			beta = min(beta, best_val);
			if (beta <= alpha || best_val == -8) {
				break;
			}
		}
		cache[state_str] = -1 * best_val;
		return best_val;
	}
}

int main() {
	GameState s(3,0,{0,0,2,2},{0,2,1,1},"000000000");
	unordered_map<string, int> cache;

	// {
	// 	std::ifstream ifs("cache.txt");
	// 	if (ifs) {
	// 		boost::archive::text_iarchive ia(ifs);
	// 		ia >> cache;
	// 	}
	// }

	int sol = alphabeta(s, cache, 10, -8, 8);

	ofstream ofs("ab_cache.txt");

	{
		boost::archive::text_oarchive oa(ofs);
		oa << cache;
	}

	cout << sol << endl;
}

