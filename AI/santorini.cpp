#include <iostream>
#include <string>
#include <vector>

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
	s.board = "230000000";
	vector<GameState> v = get_moves(s);
	cout << v.size() << endl;
	for (int i = 0; i < v.size(); ++i) {
		print_state(v[i]);
		cout << endl;
	}
}

int main() {
	test_moves();
	return 0;
}

