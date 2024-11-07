#include <macros.h>
#ifndef MINIMAX_H
#define MINIMAX_H
// new line 
#define FILENAME "bestmove.txt"
#define MAX_BOARDS 100 // Maximum number of boards we can store in memory

// Define the Move struct
struct Move {
    int row;
    int col;
};

// (new line)
// Define the BoardState struct
struct BoardState {
    int board[3][3];
    struct Move bestMove;
};

// Function declaration
struct Move findBestMove(int board[3][3]);
#endif // MINIMAX_H

int max(int a, int b);
int min(int a, int b);
struct Move findBestMove(int board[3][3]) ;
int minimax(int board[3][3], int depth, bool isMax);
int evaluate(int b[3][3]);
bool isMovesLeft(int board[3][3]);

// new line 
bool checkAndUpdateBestMove(int board[3][3], struct Move *bestMove, struct BoardState boardStates[], int count);
void writeBestMoveToFile(int board[3][3], struct Move bestMove);
int loadBoardStates(struct BoardState boardStates[]);
//void readBestMoveFromFile(int board[3][3], struct Move *bestMove);

