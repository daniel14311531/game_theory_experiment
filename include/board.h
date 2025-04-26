#ifndef BOARD_H
#define BOARD_H

#include <vector>

extern int board[4][4];
extern int whose_turn;

// Initializes the board and sets the initial turn.
void init_board();

// Prints the current state of the board.
void print_board();

// Attempts to set a rectangular area of the board to 0.
// Returns true if successful, false otherwise.
bool set_board(int x1, int y1, int x2, int y2);

// Converts the board to a 2D vector representation.
std::vector<std::vector<int>> to_vector();

// Checks if the game has ended.
bool check_end_board();

// Sets the board state using a 2D vector.
void set_board_with_vector(std::vector<std::vector<int> > state);

#endif // BOARD_H