#include <iostream>
#include "GameState.h"

using namespace std;

void test_board_to_string() {
	GameState s;
	s.dim = 3;
	s.turn = 0;
	s.p1 = {0, 0, 2, 2};
	s.p2 = {0, 2, 1, 1};
	s.board = "000000000";

	cout << s.board_to_string() << endl;
}

void test_rotate() {
	GameState s;
	s.dim = 3;
	s.turn = 0;
	s.p1 = {0, 0, 2, 2};
	s.p2 = {0, 2, 1, 1};
	s.board = "012345678";

	cout << "Initial State" << endl;
	s.print_state(cout);
	cout << endl;

	cout << "Reflect Horizontal" << endl;
	s.reflect_horizontal().print_state(cout);
	cout << endl;

	cout << "Reflect Vertical" << endl;
	s.reflect_vertical().print_state(cout);
	cout << endl;

	cout << "Reflect Left Diagonal" << endl;
	s.reflect_left_diag().print_state(cout);
	cout << endl;

	cout << "Reflect Right Diagonal" << endl;
	s.reflect_right_diag().print_state(cout);
	cout << endl;

	cout << "Rotate 90 Degrees Clockwise" << endl;
	s.reflect_horizontal().reflect_left_diag().print_state(cout);
	cout << endl;

	cout << "Rotate 180 Degrees Clockwise" << endl;
	s.reflect_horizontal().reflect_vertical().print_state(cout);
	cout << endl;

	cout << "Rotate 270 Degrees Clockwise" << endl;
	s.reflect_horizontal().reflect_right_diag().print_state(cout);
	cout << endl;
}

void test_moves() {
	GameState s;
	s.dim = 3;
	s.turn = 0;
	s.p1 = {0, 0, 2, 2};
	s.p2 = {0, 2, 1, 1};
	s.board = "000000000";
	vector<GameState> v = s.get_moves();
	cout << v.size() << endl;
	for (int i = 0; i < v.size(); ++i) {
		v[i].print_state(cout);
		cout << endl;
	}
}

int main() {
	test_rotate();
	test_board_to_string();
	test_moves();
}