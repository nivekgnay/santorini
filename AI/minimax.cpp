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

int minimax(GameState s, unordered_map<string,int>& cache) {
	if (boost::optional<int> check = s.is_game_over()) {
		return *check;
	}

	string state_str = s.board_to_string();
	if (s.turn == 0) {
		// maximizing player
		if (boost::optional<int> cached_value = s.get_cached_value(cache)) {
			cout << "Cache Hit!" << endl;
			return *cached_value;
		}

		// if no_moves left, lose by default
		int best_val = -1;
		// vector<GameState> v = get_moves(s);
		vector<GameState> v = s.get_ordered_moves(&GameState::height_heuristic);
		for (GameState next : v) {
			best_val = max(best_val, minimax(next, cache));
			if (best_val == 1) {
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

		int best_val = 1;
		// vector<GameState> v = get_moves(s);
		vector<GameState> v = s.get_ordered_moves(&GameState::height_heuristic);
		for (GameState next : v) {
			best_val = min(best_val, minimax(next, cache));
			if (best_val == -1) {
				break;
			}
		}
		cache[state_str] = -1*best_val;
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

	int sol = minimax(s, cache);

	ofstream ofs("cache.txt");

	{
		boost::archive::text_oarchive oa(ofs);
		oa << cache;
	}

	cout << sol << endl;
}