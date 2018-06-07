#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <boost/optional.hpp>

using namespace std;

struct Position {
	int row1;
	int col1;
	int row2;
	int col2;
};

struct GameState {
	int dim;
	int turn;
	Position p1;
	Position p2;
	string board;
};

bool is_occupied(int row, int col, Position p1, Position p2) {
	// Returns true if row, col equals any of the positions of p1 or p2
	if ((row == p1.row1 && col == p1.col1) ||
		(row == p1.row2 && col == p1.col2) ||
		(row == p2.row1 && col == p2.col1) ||
		(row == p2.row2 && col == p2.col2)) {
		return true;
	}
	return false;
}

bool off_board(int row, int col, int dim) {
	if (row < 0 || row >= dim || col < 0 || col >= dim) {
		return true;
	}
	return false;
}

bool is_capped(int row, int col, int dim, string board) {
	int index = row * dim + col;
	if (board[index] == '4') {
		return true;
	}
	return false;
}

bool is_too_high(int new_row, int new_col, int old_row, int old_col, int dim, string board) {
	int new_index = new_row * dim + new_col;
	int old_index = old_row * dim + old_col;

	if (board[new_index] - board[old_index] > 1) {
		return true;
	}
	return false;
}

vector<GameState> get_moves(GameState s) {
	// Return a vector of next possible game states from s
	vector<GameState> all_moves;
	Position curr;
	Position opp;
	if (s.turn == 0) {
		curr = s.p1;
		opp = s.p2;
	} else {
		curr = s.p2;
		opp = s.p1;
	}

	for (int row_offset : {-1, 0, 1}) {
		for (int col_offset: {-1, 0, 1}) {
			if (row_offset == 0 && col_offset == 0) {
				continue;
			}
			for (int piece : {0, 1}) {
				int old_row, old_col;
				if (piece == 0) {
					old_row = curr.row1;
					old_col = curr.col1;
				} else {
					old_row = curr.row2;
					old_col = curr.col2;
				}
				int new_row = old_row + row_offset;
				int new_col = old_col + col_offset;
				if (is_occupied(new_row, new_col, curr, opp) ||
					off_board(new_row, new_col, s.dim) ||
					is_capped(new_row, new_col, s.dim, s.board) ||
					is_too_high(new_row, new_col, old_row, old_col, s.dim, s.board)) {
					continue;
				}

				// try to place block
				Position new_pos = curr;
				if (piece == 0) {
					new_pos.row1 = new_row;
					new_pos.col1 = new_col;
				} else {
					new_pos.row2 = new_row;
					new_pos.col2 = new_col;
				}

				for (int row_offset1 : {-1, 0, 1}) {
					for (int col_offset1 : {-1, 0, 1}) {
						int block_row = new_row + row_offset1;
						int block_col = new_col + col_offset1;

						if (!is_occupied(block_row, block_col, new_pos, opp) &&
							!off_board(block_row, block_col, s.dim) &&
							!is_capped(block_row, block_col, s.dim, s.board)) {
							GameState new_state;
							new_state.dim = s.dim;
							new_state.turn = 1 - s.turn;
							if (s.turn == 0) {
								new_state.p1 = new_pos;
								new_state.p2 = opp;
							} else {
								new_state.p1 = opp;
								new_state.p2 = new_pos;
							}
							string new_board = s.board;
							int block_index = block_row * s.dim + block_col;
							new_board[block_index] = s.board[block_index] + 1;
							new_state.board = new_board;
							all_moves.push_back(new_state);

						}
					}
				}
			}
		}
	}
	return all_moves;
}

void print_state(GameState s) {
	cout << "Dim: " << s.dim << endl;
	cout << "Turn: " << s.turn << endl;
	cout << "P1 Positions: " << s.p1.row1 << "," << s.p1.col1 << " ";
	cout << s.p1.row2 << "," << s.p1.col2 << endl;
	cout << "P2 Positions: " << s.p2.row1 << "," << s.p2.col1 << " ";
	cout << s.p2.row2 << "," << s.p2.col2 << endl;
	for (int i = 0; i < s.dim; ++i) {
		for (int j = 0; j < s.dim; ++j) {
			if ((i == s.p1.row1 && j == s.p1.col1) ||
				(i == s.p1.row2 && j == s.p1.col2)) {
				cout << s.board[i*s.dim + j] << "* ";
			} else if ((i == s.p2.row1 && j == s.p2.col1) ||
					   (i == s.p2.row2 && j == s.p2.col2)) {
				cout << s.board[i*s.dim + j] << "^ ";
			} else {
				cout << s.board[i*s.dim + j] << "  ";
			}
		}
		cout << endl;
	}
}

void test_moves() {
	GameState s;
	s.dim = 3;
	s.turn = 0;
	s.p1 = {0, 0, 2, 2};
	s.p2 = {0, 2, 1, 1};
	s.board = "000000000";
	vector<GameState> v = get_moves(s);
	cout << v.size() << endl;
	for (int i = 0; i < v.size(); ++i) {
		print_state(v[i]);
		cout << endl;
	}
}

boost::optional<int> is_game_over(GameState s) {
	// Returns 1 if p1 wins, -1 if p2 wins, false otherwise
	int index1 = s.p1.row1 * s.dim + s.p1.col1;
	int index2 = s.p1.row2 * s.dim + s.p1.col2;
	int index3 = s.p2.row1 * s.dim + s.p2.col1;
	int index4 = s.p2.row2 * s.dim + s.p2.col2;

	if (s.board[index1] == '3' || s.board[index2] == '3') {
		return 1;
	} else if (s.board[index3] == '3' || s.board[index4] == '3') {
		return -1;
	} else {
		return boost::optional<int>();
	}
}

GameState reflect_horizontal(GameState s) {
	GameState reflect = s;

	ostringstream oss;
	for (int row = s.dim - 1; row >= 0; --row) {
		for (int col = 0; col < s.dim; ++col) {
			oss << s.board[row * s.dim + col];
		}
	}
	reflect.board = oss.str();
	Position reflect_p1 = {s.dim - s.p1.row1 - 1, s.p1.col1, 
						   s.dim - s.p1.row2 - 1, s.p1.col2};
	Position reflect_p2 = {s.dim - s.p2.row1 - 1, s.p2.col1, 
						   s.dim - s.p2.row2 - 1, s.p2.col2};
	reflect.p1 = reflect_p1;
	reflect.p2 = reflect_p2;

	return reflect;
}

GameState reflect_vertical(GameState s) {
	GameState reflect = s;

	ostringstream oss;
	for (int row = 0; row < s.dim; ++row) {
		for (int col = s.dim - 1; col >= 0; --col) {
			oss << s.board[row * s.dim + col];
		}
	}
	reflect.board = oss.str();
	Position reflect_p1 = {s.p1.row1, s.dim - s.p1.col1 - 1, 
						   s.p1.row2, s.dim - s.p1.col2 - 1};
	Position reflect_p2 = {s.p2.row1, s.dim - s.p2.col1 - 1, 
						   s.p2.row2, s.dim - s.p2.col2 - 1};
	reflect.p1 = reflect_p1;
	reflect.p2 = reflect_p2;

	return reflect;
}

GameState reflect_left_diag(GameState s) {
	GameState reflect = s;

	ostringstream oss;
	for (int col = 0; col < s.dim; ++col) {
		for (int row = 0; row < s.dim; ++row) {
			oss << s.board[row * s.dim + col];
		}
	}
	reflect.board = oss.str();
	Position reflect_p1 = {s.p1.col1, s.p1.row1, 
						   s.p1.col2, s.p1.row2};
	Position reflect_p2 = {s.p2.col1, s.p2.row1, 
						   s.p2.col2, s.p2.row2};
	reflect.p1 = reflect_p1;
	reflect.p2 = reflect_p2;

	return reflect;
}

GameState reflect_right_diag(GameState s) {
	GameState reflect = s;

	ostringstream oss;
	for (int col = s.dim - 1; col >= 0; --col) {
		for (int row = s.dim - 1; row >= 0; --row) {
			oss << s.board[row * s.dim + col];
		}
	}
	reflect.board = oss.str();
	Position reflect_p1 = {s.dim - s.p1.col1 - 1, s.dim - s.p1.row1 - 1, 
						   s.dim - s.p1.col2 - 1, s.dim - s.p1.row2 - 1};
	Position reflect_p2 = {s.dim - s.p2.col1 - 1, s.dim - s.p2.row1 - 1, 
						   s.dim - s.p2.col2 - 1, s.dim - s.p2.row2 - 1};
	reflect.p1 = reflect_p1;
	reflect.p2 = reflect_p2;

	return reflect;
}

string state_to_string(GameState s) {
	istringstream iss(s.board);
	ostringstream oss;

	for (int row = 0; row < s.dim; ++row) {
		for (int col = 0; col < s.dim; ++col) {
			char c;
			iss >> c;
			oss << c;
			if ((row == s.p1.row1 && col == s.p1.col1) ||
				(row == s.p1.row2 && col == s.p1.col2)) {
				if (s.turn == 0) {
					oss << "*";
				} else {
					oss << "^";
				}
			} else if ((row == s.p2.row1 && col == s.p2.col1) ||
					   (row == s.p2.row2 && col == s.p2.col2)) {
				if (s.turn == 0) {
					oss << "^";
				} else {
					oss << "*";
				}
			}
		}
	}
	return oss.str();
}

boost::optional<int> get_cached_value(GameState s, unordered_map<string,int>& cache) {
	string state_str = state_to_string(s);
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_horizontal = reflect_horizontal(s);
	state_str = state_to_string(s_horizontal);
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_vertical = reflect_vertical(s);
	state_str = state_to_string(s_vertical);
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_left_diag = reflect_left_diag(s);
	state_str = state_to_string(s_left_diag);
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_right_diag = reflect_right_diag(s);
	state_str = state_to_string(s_right_diag);
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_rotate1 = reflect_left_diag(s_horizontal);
	state_str = state_to_string(s_rotate1);
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_rotate2 = reflect_vertical(s_horizontal);
	state_str = state_to_string(s_rotate2);
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_rotate3 = reflect_right_diag(s_horizontal);
	state_str = state_to_string(s_rotate3);
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	return boost::optional<int>();
}

int minimax(GameState s, unordered_map<string,int>& cache, int depth) {
	if (boost::optional<int> check = is_game_over(s)) {
		return *check;
	}
	// if (check != 0) {
	// 	// cout << "Leaf node found with value " << check << ", depth: " << depth << endl;
	// 	return check;
	// }

	string state_str = state_to_string(s);
	if (s.turn == 0) {
		// maximizing player
		if (boost::optional<int> cached_value = get_cached_value(s, cache)) {
			cout << "Cache Hit!" << endl;
			return *cached_value;
		}
		// if (u1.find(state_str) != u1.end()) {
		// 	cout << "Cache Hit" << endl;
		// 	return u1[state_str];
		// }

		// if no_moves left, lose by default
		int best_val = -1;
		vector<GameState> v = get_moves(s);
		for (GameState next : v) {
			best_val = max(best_val, minimax(next, cache, depth + 1));
			if (best_val == 1) {
				break;
			}
		}
		cache[state_str] = best_val;
		return best_val;
	} else {
		if (boost::optional<int> cached_value = get_cached_value(s, cache)) {
			cout << "Cache Hit!" << endl;
			return -1 * (*cached_value);
		}

		// if (u2.find(state_str) != u2.end()) {
		// 	cout << "Cache Hit" << endl;
		// 	return u2[state_str];
		// }

		int best_val = 1;
		vector<GameState> v = get_moves(s);
		for (GameState next : v) {
			best_val = min(best_val, minimax(next, cache, depth + 1));
			if (best_val == -1) {
				break;
			}
		}
		cache[state_str] = -1*best_val;
		return best_val;
	}
}

void test_minimax() {
	GameState s;
	s.dim = 3;
	s.turn = 0;
	s.p1 = {0, 0, 2, 2};
	s.p2 = {0, 2, 1, 1};
	s.board = "000000000";

	unordered_map<string, int> cache;

	int sol = minimax(s, cache, 0);
	cout << sol << endl;
}

void test_state_to_string() {
	GameState s;
	s.dim = 3;
	s.turn = 0;
	s.p1 = {0, 0, 2, 2};
	s.p2 = {0, 2, 1, 1};
	s.board = "000000000";

	cout << state_to_string(s) << endl;
}

void test_rotate() {
	GameState s;
	s.dim = 3;
	s.turn = 0;
	s.p1 = {0, 0, 2, 2};
	s.p2 = {0, 2, 1, 1};
	s.board = "012345678";

	cout << "Initial State" << endl;
	print_state(s);
	cout << endl;

	cout << "Reflect Horizontal" << endl;
	print_state(reflect_horizontal(s));
	cout << endl;

	cout << "Reflect Vertical" << endl;
	print_state(reflect_vertical(s));
	cout << endl;

	cout << "Reflect Left Diagonal" << endl;
	print_state(reflect_left_diag(s));
	cout << endl;

	cout << "Reflect Right Diagonal" << endl;
	print_state(reflect_right_diag(s));
	cout << endl;

	cout << "Rotate 90 Degrees Clockwise" << endl;
	print_state(reflect_left_diag(reflect_horizontal(s)));
	cout << endl;

	cout << "Rotate 180 Degrees Clockwise" << endl;
	print_state(reflect_vertical(reflect_horizontal(s)));
	cout << endl;

	cout << "Rotate 270 Degrees Clockwise" << endl;
	print_state(reflect_right_diag(reflect_horizontal(s)));
	cout << endl;
}

int main() {
	test_minimax();
	return 0;
}

