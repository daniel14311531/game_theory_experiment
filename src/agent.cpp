#include <bits/stdc++.h>
#include "agent.h"

int sg[full_state], nxt_zero[full_state];
std::vector<int> edge[full_state];
int cnt_vis[full_state], cnt_win[full_state];

std::vector<std::vector<int> > decode_state(int state) {
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

int encode_state(std::vector<std::vector<int> > state) {
	int st = 0;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			st |= (state[i][j] << (i * 4 + j));
	return st;
}

std::vector<int> sub_state(int state) {
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

void init_state() {
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
	std::cout << count << "\n";
}

double calc_UCT(int state, int father) {
	if(cnt_vis[state] == 0) return DBL_MAX;
	double exploitation = (double)cnt_win[state] / cnt_vis[state];
	double exploration = C * sqrt(log(cnt_vis[father]) / cnt_vis[state]);
	return exploitation + exploration;
}

int get_best_child(int u) {
	double best = -INT_MAX;
	int v;
	for(auto nxt : edge[u]) {
		double value_v = calc_UCT(nxt, u);
		if(best < value_v) {
			best = value_v;
			v = nxt;
		}
	}
	return v;
}

bool simulation(int state, bool flag = true) {
	std::vector<int> nxt = edge[state];
	if(nxt.size() == 0) return flag;
	int u = nxt[rand() % (int)nxt.size()];
	return simulation(u, !flag);
}

std::vector<std::vector<int>> agent_option(std::vector<std::vector<int>> state) {
	int cur = encode_state(state);
	if(sg[cur]) return decode_state(nxt_zero[cur]);
	for(int T = 0; T < INTERATION; T++) {
		std::vector<std::pair<int, int> > path = {std::make_pair(cur, 1)};
		int u = cur, flag = 1, v;
		for(;;) {
			if(!cnt_vis[u]) {
				for(int i = 0; i < 100; i++) {
					++cnt_vis[u];
					bool res = simulation(u);
					if(res) {
						++cnt_win[u];
					}
				}
				for(auto p : path) {
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