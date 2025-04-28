#include <bits/stdc++.h>
#include "board.hpp"
#include "agent.hpp"

std::random_device rd;
std::mt19937 mt(rd());

void player_vs_computer() {
	system("clear");
	init_board();

	static AgentMCTS agent_mcts;
	agent_mcts.clear();
	agent_mcts.init_state();

	printf("Player first ? (yes/no)\n");
	std::string op;
	std::cin >> op;
	for(;;) {
		if(op == "yes") {
			whose_turn = 1;
			break;
		}
		if(op == "no") {
			whose_turn = 2;
			break;
		}
		system("clear");
		printf("Operate first ? (yes/no)\n");
		std::cin >> op;
	}
	for(; !check_end_board();) {
		system("clear");
		print_board();
		if(whose_turn == 1) {
			printf("Your turn: [x1 y1 x2 y2]\n");
			int x1, y1, x2, y2;
			scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
			--x1, --x2, --y1, --y2;
			bool flag = set_board(x1, y1, x2, y2);
			if(flag) {
				whose_turn = 3 - whose_turn;
			}
			else {
				printf("Invalid option: [%d %d %d %d]\n", x1 + 1, y1 + 1, x2 + 1, y2 + 1);
			}
		}
		else {
			printf("Computer turn:\n");
			std::vector<std::vector<int> > nxt = agent_mcts.agent_option(to_vector());
			set_board_with_vector(nxt);
			whose_turn = 3 - whose_turn;
		}
	}
	if(whose_turn == 1) {
		printf("Player win!\n");
	}
	else {
		printf("Computer win!\n");
	}
}

void computer_vs_computer() {
	system("clear");
	init_board();

	static AgentMCTS agent_mcts;
	agent_mcts.clear();
	agent_mcts.init_state();
	static AgentSG agent_sg;
	agent_sg.clear();
	agent_sg.init_state();
	
	printf("Computer1: AgentMCTS\n");
	printf("Computer2: AgentSG\n");
	printf("Computer1 first ? (yes/no)\n");
	std::string op;
	std::cin >> op;
	for(;;) {
		if(op == "yes") {
			whose_turn = 1;
			break;
		}
		if(op == "no") {
			whose_turn = 2;
			break;
		}
		system("clear");
		printf("Computer1: AgentMCTS\n");
		printf("Computer2: AgentSG\n");
		printf("Computer1 first ? (yes/no)\n");
		std::cin >> op;
	}
	for(; !check_end_board();) {
		printf("\n");
		printf("Computer1: AgentMCTS\n");
		printf("Computer2: AgentSG\n");
		print_board();
		if(whose_turn == 1) {
			printf("Computer1 turn:\n");
			std::vector<std::vector<int> > nxt = agent_mcts.agent_option(to_vector());
			set_board_with_vector(nxt);
			whose_turn = 3 - whose_turn;
		}
		else {
			printf("Computer2 turn:\n");
			std::vector<std::vector<int> > nxt = agent_sg.agent_option(to_vector());
			set_board_with_vector(nxt);
			whose_turn = 3 - whose_turn;
		}
	}
	printf("\n");
	printf("Computer1: AgentMCTS\n");
	printf("Computer2: AgentSG\n");
	print_board();
	if(whose_turn == 1) {
		printf("Computer1 win!\n");
	}
	else {
		printf("Computer2 win!\n");
	}
}

int main() {
	std::string op;
	printf("Player vs Computer ? (yes/no)\n");
	std::cin >> op;
	for(;;) {
		if(op == "yes") {
			break;
		}
		if(op == "no") {
			break;
		}
		system("clear");
		printf("Player vs Computer ? (yes/no)\n");
		std::cin >> op;
	}
	if(op == "yes") {
		player_vs_computer();
		return 0;
	}
	printf("Computer vs Computer ? (yes/no)\n");
	std::cin >> op;
	for(;;) {
		if(op == "yes") {
			break;
		}
		if(op == "no") {
			break;
		}
		system("clear");
		printf("Computer vs Computer ? (yes/no)\n");
		std::cin >> op;
	}
	if(op == "yes") {
		computer_vs_computer();
		return 0;
	}
	return 0;
}