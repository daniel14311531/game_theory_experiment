#include <bits/stdc++.h>
#include "board.hpp"

int board[4][4];
int whose_turn;

void init_board() {
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			board[i][j] = 1;
	whose_turn = 1;
}

void print_board() {
	printf("********\n");
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++)
			printf(" %d", board[i][j]);
		printf("\n");
	}
	printf("********\n");
}

bool set_board(int x1, int y1, int x2, int y2) {
	if(x1 != x2 && y1 != y2) return false;
	if(x1 > x2 || y1 > y2) return false;
	for(int i = x1; i <= x2; i++)
		for(int j = y1; j <= y2; j++)
			if(board[i][j] == 0)
				return false;
	for(int i = x1; i <= x2; i++)
		for(int j = y1; j <= y2; j++)
			board[i][j] = 0;
	return 1;
}

std::vector<std::vector<int> > to_vector() {
	std::vector<std::vector<int> > res;
	res.resize(4);
	for(int i = 0; i < 4; i++) {
		res[i].resize(4);
		for(int j = 0; j < 4; j++)
			res[i][j] = board[i][j];
	}
	return res;
}

bool check_end_board() {
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if(board[i][j])
				return false;
	return true;
}

void set_board_with_vector(std::vector<std::vector<int> > state) {
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			board[i][j] = state[i][j];
}