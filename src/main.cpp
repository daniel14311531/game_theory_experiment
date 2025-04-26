#include <bits/stdc++.h>
#include "board.h"
#include "agent.h"

int main() {
	init_board();
	init_state();
	printf("Operate first ? (yes/no)\n");
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
			printf("Agent turn:\n");
			std::vector<std::vector<int> > nxt = agent_option(to_vector());
			set_board_with_vector(nxt);
			whose_turn = 3 - whose_turn;
		}
	}
	if(whose_turn == 1) {
		printf("Player win!\n");
	}
	else {
		printf("Agent win!\n");
	}
	return 0;
}