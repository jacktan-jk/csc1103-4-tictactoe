/**
 * @file minimax.h
 * @author jacktan-jk
 * @brief Header file for the Tic-Tac-Toe Minimax algorithm.
 * @version 1.0
 * @date 2024-11-12
 * 
 * @copyright Copyright (c) 2024
 * 
 * This file contains function declarations, structure definitions, and constants for 
 * implementing the Minimax algorithm in a Tic-Tac-Toe game. It includes functions for 
 * evaluating board states, calculating the best move for the bot, and checking if any 
 * moves are left on the board.
 */

#ifndef MINIMAX_H // Start of include guard
#define MINIMAX_H

#include <macros.h>  /**< Include macro definitions */

/** 
 * @brief Represents the position of a move on the Tic-Tac-Toe board.
 * 
 * This structure defines the row and column for a move made by the bot or player.
 */
struct Position
{
    int row; /**< Row index of the move */
    int col; /**< Column index of the move */
};

/** 
 * @brief Stores the current state of the Tic-Tac-Toe board along with the best move.
 * 
 * This structure holds a 3x3 board array and the best move associated with that 
 * board state. It is used for storing and comparing previous board states and their 
 * corresponding optimal moves.
 */
struct BoardState
{
    int board[3][3]; /**< The Tic-Tac-Toe board */
    struct Position bestMove; /**< The best move for the bot */
};

#define FILE_BESTMOV "resources/bestmove.txt" /**< Path to the file storing best moves */
#define MAX_BOARDS 10000 /**< Maximum number of boards to store in memory */

/** 
 * @brief Compares two integers and returns the larger of the two.
 * 
 * This function is used by the Minimax algorithm to maximize the score for the 
 * maximizer's move (usually the bot).
 * 
 * @param a The first integer.
 * @param b The second integer.
 * @return The larger of the two values.
 */
int max(int a, int b);

/** 
 * @brief Compares two integers and returns the smaller of the two.
 * 
 * This function is used by the Minimax algorithm to minimize the score for the 
 * minimizer's move (usually the player).
 * 
 * @param a The first integer.
 * @param b The second integer.
 * @return The smaller of the two values.
 */
int min(int a, int b);

/** 
 * @brief Finds the best move for the bot on the given board.
 * 
 * This function uses the Minimax algorithm to evaluate all possible moves and 
 * returns the best move for the bot to make based on the current board state.
 * 
 * @param board The current state of the Tic-Tac-Toe board.
 * @return The best move for the bot.
 * @see minimax, evaluate
 */
struct Position findBestMove(int board[3][3]);

/** 
 * @brief Recursively evaluates all possible moves using the Minimax algorithm.
 * 
 * This function determines the best score for the current board state based on the 
 * depth of the search tree and whether it is the maximizer's or minimizer's turn.
 * 
 * @param board The current state of the Tic-Tac-Toe board.
 * @param depth The current depth in the Minimax search tree.
 * @param isMax True if it's the maximizer’s turn (bot), false if it's the minimizer's turn (player).
 * @return The best score for the current board state.
 * @see evaluate
 */
int minimax(int board[3][3], int depth, bool isMax);

/** 
 * @brief Evaluates the current state of the Tic-Tac-Toe board.
 * 
 * This function checks for a win or draw condition and returns a score based on the result. 
 * A win for the bot is +10, a win for the player is -10, and a tie is 0.
 * 
 * @param board The current state of the Tic-Tac-Toe board.
 * @return A score representing the result of the evaluation: +10 for a bot win, 
 *         -10 for a player win, and 0 for a tie.
 * @see minimax
 */
int evaluate(int b[3][3]);

/** 
 * @brief Checks if there are any moves left on the Tic-Tac-Toe board.
 * 
 * This function checks if there are any empty cells on the board to make a move.
 * 
 * @param board The current state of the Tic-Tac-Toe board.
 * @return True if there are moves left (i.e., empty cells), false if the board is full.
 * @see minimax
 */
bool isMovesLeft(int board[3][3]);

/** 
 * @brief Checks and updates the best move for a board state.
 * 
 * This function checks if the current board state matches any previously stored board 
 * states. If a match is found, it updates the best move for that board.
 * 
 * @param board The current state of the Tic-Tac-Toe board.
 * @param bestMove A pointer to the best move that will be updated.
 * @param boardStates An array of stored board states.
 * @param count The number of board states in memory.
 * @return True if a matching board state is found and the best move is updated, false otherwise.
 * @see loadBoardStates
 */
bool checkAndUpdateBestMove(int board[3][3], struct Position *bestMove, struct BoardState boardStates[], int count);

/** 
 * @brief Writes the best move for the current board to a file.
 * 
 * This function appends the current board state and its corresponding best move 
 * to the file for future reference and lookup.
 * 
 * @param board The current state of the Tic-Tac-Toe board.
 * @param bestMove The best move for the current board state.
 * @see checkAndUpdateBestMove
 */
void writeBestMoveToFile(int board[3][3], struct Position bestMove);

/** 
 * @brief Loads the board states from the file.
 * 
 * This function loads previously saved board states and their corresponding best 
 * moves from the file into memory.
 * 
 * @param boardStates An array to store the loaded board states.
 * @return The number of board states loaded from the file.
 * @see writeBestMoveToFile
 */
int loadBoardStates(struct BoardState boardStates[]);

/** 
 * @brief Prints the contents of the best move file.
 * 
 * This function reads and prints the contents of the file that stores the best moves 
 * for various board states.
 */
void printFileContents();

#endif // MINIMAX_H // End of include guard