#ifndef AGENT_HPP
#define AGENT_HPP

#include <bits/stdc++.h>

// the count of states
const int full_state = (1 << 16) + 10;

// random seed
extern std::random_device rd;
extern std::mt19937 mt;

class Agent {
protected:
	// sg function and zero substate
	int sg[full_state], nxt_zero[full_state];
	// edges in the directed acyclic state graph
	std::vector<int> edge[full_state];

	// decode the binary state to 2D vector
	std::vector<std::vector<int>> decode_state(int state);
	// encode the 2D vector to binary state
	int encode_state(std::vector<std::vector<int>> state);
	// get all the substate of the current state
	std::vector<int> sub_state(int state);

public:
	Agent();
	virtual ~Agent();
	// clear the state graph
	virtual void clear();
	// initialize the state graph
	virtual void init_state();
	// get the next option of the agent
	virtual std::vector<std::vector<int>> agent_option(std::vector<std::vector<int>> state);
};

class AgentMCTS : public Agent {
protected:
	// constants for UCT
	const double C = 1.414;
	// number of iterations
	const int INTERATION = 10000;
	// number of simulations and wins
	int cnt_vis[full_state], cnt_win[full_state];
	// count the number of visits from the current node to a certain child
	std::vector<int> cnt_to_child[full_state];
	// the map between child and its id
	std::map<int, int> child_to_id[full_state];

	// UCT calculation
	double calc_UCT(int state, int father);
	// simulate the game from the current state (randomly)
	bool simulation(int state, bool flag);
	// get the best child of the current state
	int get_best_child(int u);

public:
	AgentMCTS();
	~AgentMCTS();
	// clear the state graph
	void clear() override;
	// get the next option of the agent
	std::vector<std::vector<int>> agent_option(std::vector<std::vector<int>> state) override;
};

class AgentSG : public Agent {
protected:
	// a substate with sg function value 0
	int nxt_non_zero[full_state];
	// the probability of the non-zero substate
	double non_zero_prob[full_state];

public:
	AgentSG();
	~AgentSG();
	// clear the state graph
	void clear() override;
	// initialize the state graph
	void init_state() override;
	// get the next option of the agent
	std::vector<std::vector<int>> agent_option(std::vector<std::vector<int>> state) override;
};

#endif // AGENT_HPP