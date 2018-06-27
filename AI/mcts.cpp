#include <unordered_map>
#include <vector>
#include <limits>
#include <cmath>
#include <memory>
#include <stdexcept>
#include "GameState.h"

using namespace std;

struct NodeInfo {
	int Q;
	int N;
	bool is_leaf;
}; 

class MctsNode : public enable_shared_from_this<MctsNode> {
public:
	MctsNode(GameState s, NodeInfo i): state{s}, info{i} {} 

	vector<shared_ptr<MctsNode>> children;
	weak_ptr<MctsNode> parent;
	GameState state;
	NodeInfo info;
	
	double compute_uct(double const c) {
		// Returns UCT value. Assumed to be infinity when it the node is unvisited
		shared_ptr<MctsNode> tmp_parent = parent.lock();
		if (tmp_parent) {
			if (info.N == 0 || tmp_parent == nullptr) {
				return numeric_limits<double>::max();
			} else {
				return (info.Q / (double) info.N) + 2 * c * sqrt(2 * log(tmp_parent->info.N) / info.N); 
			}
		}
		throw runtime_error("parent ptr does not exist");
		
	}

	void expand() {
		// populate children
		vector<GameState> child_states = state.get_moves();
		for (GameState child_state : child_states) {
			shared_ptr<MctsNode> child;
			if (child_state.is_game_over()) {
				child = shared_ptr<MctsNode>(new MctsNode(child_state, {0, 0, true}));
				child->parent = shared_from_this();
			} else {
				child = shared_ptr<MctsNode>(new MctsNode(child_state, {0, 0, false}));
				child->parent = shared_from_this();
			}
			children.push_back(std::move(child));
		}
	}
};

void dfs_print(shared_ptr<MctsNode> node) {
	node->state.print_state(cout);
	for (auto child : node->children) {
		dfs_print(child);
	}
}

int tree_size(shared_ptr<MctsNode> node) {
	if (node->children.size() == 0) {
		return 1;
	}
	int total = 0;
	for (auto child : node->children) {
		total += tree_size(child);
	}
	return total + 1;
}

void test_tree() {
	GameState init(3, 0, {0, 0, 2, 2}, {0, 2, 2, 0}, "000000000");
	shared_ptr<MctsNode> root = shared_ptr<MctsNode>(new MctsNode(init, {0,0, false}));
	root->expand();
	
	assert(tree_size(root) == 27);
}

int main() {
	GameState init(3, 0, {0, 0, 2, 2}, {0, 2, 2, 0}, "000000000");
	shared_ptr<MctsNode> root = shared_ptr<MctsNode>(new MctsNode(init, {0,0, false}));

	root->expand();
	root->children[0]->expand();

	test_tree();

	return 0;
}




