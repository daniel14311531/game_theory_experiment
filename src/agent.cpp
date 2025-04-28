#include <bits/stdc++.h>
#include "agent.hpp"

Agent::Agent() {
	for(int i = 0; i < full_state; i++) {
		sg[i] = 0;
		nxt_zero[i] = 0;
		edge[i].clear();
	}
}

Agent::~Agent() = default;
	
std::vector<std::vector<int> > Agent::decode_state(int state) {
	std::vector<int> a(16);
	for(int i = 0; i < 16; i++)
		a[i] = ((state >> i) & 1);
	std::vector<std::vector<int> > st;
	st.resize(4);
	for(int i = 0; i < 4; i++) {
		st[i].resize(4);
		for(int j = 0; j < 4; j++)
			st[i][j] = a[i * 4 + j];
	}
	return st;
}

int Agent::encode_state(std::vector<std::vector<int> > state) {
	int st = 0;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			st |= (state[i][j] << (i * 4 + j));
	return st;
}

std::vector<int> Agent::sub_state(int state) {
	std::vector<std::vector<int> > cur = decode_state(state), tmp;
	std::vector<int> res;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++) {
			if(!cur[i][j]) continue;
			tmp = cur, tmp[i][j] = 0;
			res.push_back(encode_state(tmp));
			int ti = i, tj = j;
			for(;;) {
				ti += 1;
				if(ti >= 4 || !tmp[ti][tj]) break;
				tmp[ti][tj] = 0;
				res.push_back(encode_state(tmp));
			}
			ti = i, tj = j;
			tmp = cur, tmp[i][j] = 0;
			for(;;) {
				tj += 1;
				if(tj >= 4 || !tmp[ti][tj]) break;
				tmp[ti][tj] = 0;
				res.push_back(encode_state(tmp));
			}
		}
	return res;
}

void Agent::clear() {
	for(int i = 0; i < full_state; i++) {
		sg[i] = 0;
		nxt_zero[i] = 0;
		edge[i].clear();
	}
}

void Agent::init_state() {
	int count = 0;
	int full = (1 << 16) - 1;
	sg[0] = full_state + 10;
	for(int i = 1; i <= full; i++) {
		std::vector<int> nxt = sub_state(i);
		edge[i] = nxt;
		count += nxt.size();
		if(nxt.size() == 1 && nxt[0] == 0) {
			sg[i] = 0;
			continue;
		}
		std::set<int> cnt;
		for(auto v : nxt) cnt.insert(sg[v]);
		int cur = 0;
		for(; cnt.find(cur) != cnt.end();) {
			++cur;
		}
		sg[i] = cur;
		if(sg[i] != 0) {
			for(auto v : nxt) {
				if(sg[v] == 0) {
					nxt_zero[i] = v;
				}
			}
		}
	}
	// std::cout << count << "\n";
}

std::vector<std::vector<int>> Agent::agent_option(std::vector<std::vector<int>> state) {
	int cur = encode_state(state);
	if(sg[cur]) return decode_state(nxt_zero[cur]);
	// commit suiside
	return decode_state(0);
}

AgentMCTS::AgentMCTS() {
	for(int i = 0; i < full_state; i++) {
		sg[i] = 0;
		nxt_zero[i] = 0;
		cnt_vis[i] = 0;
		cnt_win[i] = 0;
		edge[i].clear();
		cnt_to_child[i].clear();
		child_to_id[i].clear();
	}
}

AgentMCTS::~AgentMCTS() = default;

void AgentMCTS::clear() {
	for(int i = 0; i < full_state; i++) {
		sg[i] = 0;
		nxt_zero[i] = 0;
		cnt_vis[i] = 0;
		cnt_win[i] = 0;
		edge[i].clear();
		cnt_to_child[i].clear();
		child_to_id[i].clear();
	}
}

double AgentMCTS::calc_UCT(int state, int father) {
	if(cnt_vis[state] == 0) return INT_MAX;
	// use more precise value
	double exploitation = (double)cnt_win[state] / cnt_vis[state];
	// the accurate count of visits from father to child
	int acc_father_to_child = cnt_to_child[father][child_to_id[father][state]];
	double exploration = C * sqrt(log(cnt_vis[father]) / acc_father_to_child);
	return -exploitation + exploration;
}

bool AgentMCTS::simulation(int state, bool flag = true) {
	if(edge[state].size() == 0) return flag;
	int u = edge[state][abs((int)mt()) % (int)edge[state].size()];
	return simulation(u, !flag);
}

int AgentMCTS::get_best_child(int u) {
	double best = 0;
	int v = -1;
	for(auto nxt : edge[u]) {
		double value_v = calc_UCT(nxt, u);
		if(v == -1 || best < value_v) {
			best = value_v;
			v = nxt;
		}
	}
	return v;
}

std::vector<std::vector<int>> AgentMCTS::agent_option(std::vector<std::vector<int>> state) {
	int cur = encode_state(state);
	if(sg[cur]) return decode_state(nxt_zero[cur]);
	for(int T = 0; T < INTERATION; T++) {
		std::vector<std::pair<int, int> > path = {std::make_pair(cur, 1)};
		int u = cur, flag = 1, v;
		for(;;) {
			if(!cnt_vis[u] || u == 0) {
				if(cnt_to_child[u].size() != edge[u].size()) {
					cnt_to_child[u].resize(edge[u].size());
					for(int i = 0; i < (int)cnt_to_child[u].size(); i++) {
						child_to_id[u][edge[u][i]] = i;
					}
				}
				for(int i = 0; i < 100; i++) {
					++cnt_vis[u];
					bool res = simulation(u);
					if(res) {
						++cnt_win[u];
					}
				}
				// The theoretical value of cnt_vis[u] should be 100
				for(int i = 0; i < (int)path.size(); i++) {
					std::pair<int, int> p = path[i];
					if(i) {
						int father = path[i - 1].first;
						int son = path[i].first;
						cnt_to_child[father][child_to_id[father][son]] += cnt_vis[u];
					}
					if(p.first == u) continue;
					cnt_vis[p.first] += cnt_vis[u];
					if(flag == p.second) {
						cnt_win[p.first] += cnt_win[u];
					}
					else {
						cnt_win[p.first] += cnt_vis[u] - cnt_win[u];
					}
				}
				break;
			}
			v = get_best_child(u);
			path.push_back(std::make_pair(v, !flag));
			u = v, flag = !flag;
		}
	}
	return decode_state(get_best_child(cur));
}

AgentSG::AgentSG() {
	for(int i = 0; i < full_state; i++) {
		sg[i] = 0;
		nxt_zero[i] = 0;
		nxt_non_zero[i] = 0;
		non_zero_prob[i] = 0;
		edge[i].clear();
	}
}

AgentSG::~AgentSG() = default;

void AgentSG::clear() {
	for(int i = 0; i < full_state; i++) {
		sg[i] = 0;
		nxt_zero[i] = 0;
		nxt_non_zero[i] = 0;
		non_zero_prob[i] = 0;
		edge[i].clear();
	}
}

void AgentSG::init_state() {
	int count = 0;
	int full = (1 << 16) - 1;
	sg[0] = full_state + 10;
	for(int i = 1; i <= full; i++) {
		std::vector<int> nxt = sub_state(i);
		edge[i] = nxt;
		count += nxt.size();
		if(nxt.size() == 1 && nxt[0] == 0) {
			sg[i] = 0;
			continue;
		}
		std::set<int> cnt;
		for(auto v : nxt) cnt.insert(sg[v]);
		int cur = 0;
		for(; cnt.find(cur) != cnt.end();) {
			++cur;
		}
		sg[i] = cur;
		if(sg[i] != 0) {
			for(auto v : nxt) {
				if(sg[v] == 0) {
					nxt_zero[i] = v;
				}
				else if(sg[v] == sg[i]) {
					nxt_non_zero[i] = v;
				}
			}
		}
	}
	for(int i = 0; i <= full; i++) {
		int cnt0 = 0, cnt1 = 0;
		for(auto v : edge[i]) {
			if(sg[v] == 0) ++cnt0;
			else ++cnt1;
		}
		non_zero_prob[i] = (double)cnt1 / (cnt0 + cnt1);

		if(sg[i] == 0) {
			int res = -1;
			for(auto v : edge[i]) {
				if(res == -1 || non_zero_prob[v] < non_zero_prob[res]) {
					res = v;
				}
			}
			if(res == -1) res = 0;
			nxt_non_zero[i] = res;
		}
	}
}

std::vector<std::vector<int>> AgentSG::agent_option(std::vector<std::vector<int>> state) {
	int cur = encode_state(state);
	if(sg[cur]) return decode_state(nxt_zero[cur]);
	if(nxt_non_zero[cur]) return decode_state(nxt_non_zero[cur]);
	return decode_state(0);
}