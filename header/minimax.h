#ifndef MINIMAX_H
#define MINIMAX_H

#include <macros.h>

#define FILE_BESTMOV "resources/bestmove.txt"
#define MAX_BOARDS 10000 // Maximum number of boards we can store in memory

// Define the BoardState struct
struct BoardState {
    int board[3][3];
    struct Position bestMove;
};

int max(int a, int b);
int min(int a, int b);
struct Position findBestMove(int board[3][3]) ;
int minimax(int board[3][3], int depth, bool isMax);
int evaluate(int b[3][3]);
bool isMovesLeft(int board[3][3]);

bool checkAndUpdateBestMove(int board[3][3], struct Position *bestMove, struct BoardState boardStates[], int count);
void writeBestMoveToFile(int board[3][3], struct Position bestMove);
int loadBoardStates(struct BoardState boardStates[]);
void printFileContents();

#endif // MINIMAX_H
