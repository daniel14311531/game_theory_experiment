#ifndef AGENT_H
#define AGENT_H

#include <bits/stdc++.h>

const int full_state = (1 << 16) + 10;
const double C = 1.414;
const int INTERATION = 10000;
extern int sg[full_state], nxt_zero[full_state];
extern std::vector<int> edge[full_state];

// first hand win rate.
extern int cnt_vis[full_state], cnt_win[full_state];

// decode the binary state to 4 * 4 map.
std::vector<std::vector<int> > decode_state(int state);

// encode the 4 * 4 map to binary state.
int encode_state(std::vector<std::vector<int> > state);

// get the next state by removing a 1 * n(or n * 1) block from the current state.
std::vector<int> sub_state(int state);

// get the best option by agent.
std::vector<std::vector<int>> agent_option(std::vector<std::vector<int>> state);

// initialize the state space.
void init_state();

// UCT function.
double calc_UCT(int state, int father);

// get the best child node.
int get_best_child(int u);

// simulate the game from the current state.
bool simulation(int state, bool flag);

#endif // AGENT_H