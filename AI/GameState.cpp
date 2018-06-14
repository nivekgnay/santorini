#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <boost/optional.hpp>
#include "GameState.h"

using namespace std;

GameState::GameState() : dim{0}, turn{0}, p1{0,0,0,0}, p2{0,0,0,0}, board{""} {}
GameState::GameState(int dim, int turn, Position p1, Position p2, string board) :
		dim{dim}, turn{turn}, p1{p1}, p2{p2}, board{board} {}

bool GameState::is_occupied(int row, int col) const {
	// Returns true if row, col equals any of the positions of p1 or p2
	if ((row == p1.row1 && col == p1.col1) ||
		(row == p1.row2 && col == p1.col2) ||
		(row == p2.row1 && col == p2.col1) ||
		(row == p2.row2 && col == p2.col2)) {
		return true;
	}
	return false;
}

bool GameState::is_occupied(int row, int col, Position p1, Position p2) const {
	// Returns true if row, col equals any of the positions of p1 or p2
	if ((row == p1.row1 && col == p1.col1) ||
		(row == p1.row2 && col == p1.col2) ||
		(row == p2.row1 && col == p2.col1) ||
		(row == p2.row2 && col == p2.col2)) {
		return true;
	}
	return false;
}

bool GameState::off_board(int row, int col) const {
	if (row < 0 || row >= dim || col < 0 || col >= dim) {
		return true;
	}
	return false;
}

bool GameState::is_capped(int row, int col) const {
	int index = row * dim + col;
	if (board[index] == '4') {
		return true;
	}
	return false;
}

bool GameState::is_too_high(int new_row, int new_col, int old_row, int old_col) const {
	int new_index = new_row * dim + new_col;
	int old_index = old_row * dim + old_col;

	if (board[new_index] - board[old_index] > 1) {
		return true;
	}
	return false;
}

boost::optional<int> GameState::is_game_over() const {
	// Returns 1 if p1 wins, -1 if p2 wins, false otherwise
	int index1 = p1.row1 * dim + p1.col1;
	int index2 = p1.row2 * dim + p1.col2;
	int index3 = p2.row1 * dim + p2.col1;
	int index4 = p2.row2 * dim + p2.col2;

	if (board[index1] == '3' || board[index2] == '3') {
		return 1;
	} else if (board[index3] == '3' || board[index4] == '3') {
		return -1;
	} else {
		return boost::optional<int>();
	}
}

vector<GameState> GameState::get_moves() const {
	// Return a vector of next possible game states from s
	vector<GameState> all_moves;
	Position curr;
	Position opp;
	if (turn == 0) {
		curr = p1;
		opp = p2;
	} else {
		curr = p2;
		opp = p1;
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
					off_board(new_row, new_col) ||
					is_capped(new_row, new_col) ||
					is_too_high(new_row, new_col, old_row, old_col)) {
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
							!off_board(block_row, block_col) &&
							!is_capped(block_row, block_col)) {
							string new_board = board;
							int block_index = block_row * dim + block_col;
							new_board[block_index] = board[block_index] + 1;

							GameState new_state;
							if (turn == 0) {
								new_state = GameState(dim, 1-turn, new_pos, opp, new_board);
							} else {
								new_state = GameState(dim, 1-turn, opp, new_pos, new_board);
							}

							all_moves.push_back(new_state);


							// GameState new_state;
							// new_state.dim = dim;
							// new_state.turn = 1 - turn;
							// if (turn == 0) {
							// 	new_state.p1 = new_pos;
							// 	new_state.p2 = opp;
							// } else {
							// 	new_state.p1 = opp;
							// 	new_state.p2 = new_pos;
							// }
							// string new_board = board;
							// int block_index = block_row * dim + block_col;
							// new_board[block_index] = board[block_index] + 1;
							// new_state.board = new_board;
							// all_moves.push_back(new_state);

						}
					}
				}
			}
		}
	}
	return all_moves;
}

int GameState::height_heuristic() const {
	// Heuristic with respect to player 1
	int index1 = p1.row1 * dim + p1.col1;
	int index2 = p1.row2 * dim + p1.col2;

	int index3 = p2.row1 * dim + p2.col1;
	int index4 = p2.row2 * dim + p2.col2;

	return board[index1] + board[index2] - board[index3] - board[index4];
}

vector<GameState> GameState::get_ordered_moves(function<int(GameState)> h) const {
	vector<GameState> all_moves = get_moves();
	if (turn == 0) {
		sort(all_moves.begin(), all_moves.end(), 
			[&h](const GameState& a, const GameState& b) -> bool {
				return h(a) > h(b);
			});
	} else {
		sort(all_moves.begin(), all_moves.end(), 
			[&h](const GameState& a, const GameState& b) -> bool {
				return h(a) < h(b);
			});
	}
	return all_moves;
}

GameState GameState::reflect_horizontal() const {
	GameState reflect = *this;

	ostringstream oss;
	for (int row = dim - 1; row >= 0; --row) {
		for (int col = 0; col < dim; ++col) {
			oss << board[row * dim + col];
		}
	}
	reflect.board = oss.str();
	Position reflect_p1 = {dim - p1.row1 - 1, p1.col1, 
						   dim - p1.row2 - 1, p1.col2};
	Position reflect_p2 = {dim - p2.row1 - 1, p2.col1, 
						   dim - p2.row2 - 1, p2.col2};
	reflect.p1 = reflect_p1;
	reflect.p2 = reflect_p2;

	return reflect;
}

GameState GameState::reflect_vertical() const {
	GameState reflect = *this;

	ostringstream oss;
	for (int row = 0; row < dim; ++row) {
		for (int col = dim - 1; col >= 0; --col) {
			oss << board[row * dim + col];
		}
	}
	reflect.board = oss.str();
	Position reflect_p1 = {p1.row1, dim - p1.col1 - 1, 
						   p1.row2, dim - p1.col2 - 1};
	Position reflect_p2 = {p2.row1, dim - p2.col1 - 1, 
						   p2.row2, dim - p2.col2 - 1};
	reflect.p1 = reflect_p1;
	reflect.p2 = reflect_p2;

	return reflect;
}

GameState GameState::reflect_left_diag() const {
	GameState reflect = *this;

	ostringstream oss;
	for (int col = 0; col < dim; ++col) {
		for (int row = 0; row < dim; ++row) {
			oss << board[row * dim + col];
		}
	}
	reflect.board = oss.str();
	Position reflect_p1 = {p1.col1, p1.row1, 
						   p1.col2, p1.row2};
	Position reflect_p2 = {p2.col1, p2.row1, 
						   p2.col2, p2.row2};
	reflect.p1 = reflect_p1;
	reflect.p2 = reflect_p2;

	return reflect;
}

GameState GameState::reflect_right_diag() const {
	GameState reflect = *this;

	ostringstream oss;
	for (int col = dim - 1; col >= 0; --col) {
		for (int row = dim - 1; row >= 0; --row) {
			oss << board[row * dim + col];
		}
	}
	reflect.board = oss.str();
	Position reflect_p1 = {dim - p1.col1 - 1, dim - p1.row1 - 1, 
						   dim - p1.col2 - 1, dim - p1.row2 - 1};
	Position reflect_p2 = {dim - p2.col1 - 1, dim - p2.row1 - 1, 
						   dim - p2.col2 - 1, dim - p2.row2 - 1};
	reflect.p1 = reflect_p1;
	reflect.p2 = reflect_p2;

	return reflect;
}

string GameState::board_to_string() const {
	istringstream iss(board);
	ostringstream oss;

	for (int row = 0; row < dim; ++row) {
		for (int col = 0; col < dim; ++col) {
			char c;
			iss >> c;
			oss << c;
			if ((row == p1.row1 && col == p1.col1) ||
				(row == p1.row2 && col == p1.col2)) {
				if (turn == 0) {
					oss << "*";
				} else {
					oss << "^";
				}
			} else if ((row == p2.row1 && col == p2.col1) ||
					   (row == p2.row2 && col == p2.col2)) {
				if (turn == 0) {
					oss << "^";
				} else {
					oss << "*";
				}
			}
		}
	}
	return oss.str();
}

boost::optional<int> GameState::get_cached_value(unordered_map<string,int>& cache) const {
	string state_str = board_to_string();
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_horizontal = reflect_horizontal();
	state_str = s_horizontal.board_to_string();
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_vertical = reflect_vertical();
	state_str = s_vertical.board_to_string();
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_left_diag = reflect_left_diag();
	state_str = s_left_diag.board_to_string();
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_right_diag = reflect_right_diag();
	state_str = s_right_diag.board_to_string();
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_rotate1 = s_horizontal.reflect_left_diag();
	state_str = s_rotate1.board_to_string();
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_rotate2 = s_horizontal.reflect_vertical();
	state_str = s_rotate2.board_to_string();
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	GameState s_rotate3 = s_horizontal.reflect_right_diag();
	state_str = s_rotate3.board_to_string();
	if (cache.find(state_str) != cache.end()) {
		return cache[state_str];
	}

	return boost::optional<int>();
}

void GameState::print_state(ostream os) const {
	os << "Dim: " << dim << endl;
	os << "Turn: " << turn << endl;
	os << "P1 Positions: " << p1.row1 << "," << p1.col1 << " ";
	os << p1.row2 << "," << p1.col2 << endl;
	os << "P2 Positions: " << p2.row1 << "," << p2.col1 << " ";
	os << p2.row2 << "," << p2.col2 << endl;
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			if ((i == p1.row1 && j == p1.col1) ||
				(i == p1.row2 && j == p1.col2)) {
				os << board[i*dim + j] << "* ";
			} else if ((i == p2.row1 && j == p2.col1) ||
					   (i == p2.row2 && j == p2.col2)) {
				os << board[i*dim + j] << "^ ";
			} else {
				os << board[i*dim + j] << "  ";
			}
		}
		os << endl;
	}
}
