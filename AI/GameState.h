#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <boost/optional.hpp>

using namespace std;

struct Position {
	int row1;
	int col1;
	int row2;
	int col2;
};

class GameState {
public:
	GameState(int, int, Position, Position, string);
	GameState();
	int dim;
	int turn;
	Position p1;
	Position p2;
	string board;
	bool is_occupied(int, int) const;
	bool is_occupied(int, int, Position, Position) const;
	bool off_board(int, int) const;
	bool is_capped(int, int) const;
	bool is_too_high(int, int, int, int) const;
	boost::optional<int> is_game_over() const;
	
	vector<GameState> get_moves() const;
	int height_heuristic() const;
	vector<GameState> get_ordered_moves() const;
	vector<GameState> get_ordered_moves(function<int(GameState)>) const;

	boost::optional<int> get_cached_value(unordered_map<string,int>&) const;

	GameState reflect_horizontal() const;
	GameState reflect_vertical() const;
	GameState reflect_left_diag() const;
	GameState reflect_right_diag() const;

	string board_to_string() const;
	void print_state(ostream) const;
};