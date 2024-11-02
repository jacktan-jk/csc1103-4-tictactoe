#include <macros.h>
#ifndef MINIMAX_H
#define MINIMAX_H
// Define the Move struct
struct Move {
    int row;
    int col;
};

// Function declaration
struct Move findBestMove(int board[3][3]);
#endif // MINIMAX_H

int max(int a, int b);
int min(int a, int b);
struct Move findBestMove(int board[3][3]);
int minimax(int board[3][3], int depth, bool isMax);
int evaluate(int b[3][3]);
bool isMovesLeft(int board[3][3]);