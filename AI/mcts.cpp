#include <unordered_map>
#include <vector>
#include <limits>
#include <cmath>
#include <memory>
#include "GameState.h"

using namespace std;

struct NodeInfo {
	int Q;
	int N;
	bool is_leaf;
}; 

class MctsNode {
public:
	MctsNode(GameState s, NodeInfo i): state{s}, info{i} {} 

	vector<unique_ptr<MctsNode>> children;
	unique_ptr<MctsNode> parent;
	GameState state;
	NodeInfo info;
	
	double compute_uct(double const c) {
		// Returns UCT value. Assumed to be infinity when it the node is unvisited, 
		if (info.N == 0 || parent == nullptr) {
			return numeric_limits<double>::max();
		} else {
			return (info.Q / (double) info.N) + 2 * c * sqrt(2 * log(parent->info.N) / info.N); 
		}
	}

	void expand() {
		// populate children
		vector<GameState> child_states = state.get_moves();
		for (GameState child_state : child_states) {
			unique_ptr<MctsNode> child;
			if (child_state.is_game_over()) {
				child = unique_ptr<MctsNode>(new MctsNode(child_state, {0, 0, true}));
			} else {
				child = unique_ptr<MctsNode>(new MctsNode(child_state, {0, 0, false}));
			}
			children.push_back(std::move(child));
		}
	}
};

int main() {
	GameState init(3, 0, {0, 0, 2, 2}, {0, 2, 2, 0}, "000000000");
	MctsNode root(init, {0, 0, false});
	
	root.parent = nullptr;

	root.expand();
	return 0;
}