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
#include <elapsedTime.h>

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
 * @brief Returns the maximum of two integers.
 *
 * This function computes the maximum of two integers using either:
 * - A standard C implementation when `DISABLE_ASM` is defined.
 * - Optimized assembly instructions for either AArch64 or x86 platforms.
 *
 * The choice of implementation depends on the platform and preprocessor directives.
 *
 * @param a The first integer to compare.
 * @param b The second integer to compare.
 * @return The greater of the two integers.
 *
 * @note If `DISABLE_ASM` is defined, the function uses pure C logic.
 * @note For AArch64 platforms, the function uses assembly instructions with conditional selection.
 * @note For x86 platforms, the function uses assembly instructions with register manipulation and branching.
 *
 * Example usage:
 * @code
 * int result = max(10, 20);
 * // result now holds the value 20.
 * @endcode
 */
int max(int a, int b);

/**
 * @brief Returns the minimum of two integers.
 *
 * This function computes the minimum of two integers using either:
 * - A standard C implementation when `DISABLE_ASM` is defined.
 * - Optimized assembly instructions for either AArch64 or x86 platforms.
 *
 * The choice of implementation depends on the platform and preprocessor directives.
 *
 * @param a The first integer to compare.
 * @param b The second integer to compare.
 * @return The smaller of the two integers.
 *
 * @note If `DISABLE_ASM` is defined, the function uses pure C logic.
 * @note For AArch64 platforms, the function uses assembly instructions with conditional selection.
 * @note For x86 platforms, the function uses assembly instructions with register manipulation and branching.
 *
 * Example usage:
 * @code
 * int result = min(10, 20);
 * // result now holds the value 10.
 * @endcode
 */
int min(int a, int b);

/**  
 * @brief Finds the best move for the bot in the Tic-Tac-Toe game.
 * 
 * This function first checks if the best move is already stored in memory by 
 * looking through previous board states. If the move is found, it is returned. 
 * If no best move is found in memory, it traverses all the empty cells on the 
 * board, evaluates the potential moves using the minimax algorithm, and returns 
 * the optimal move.
 * 
 * @param board A 3x3 array representing the current Tic-Tac-Toe board.
 * 
 * @return The best move for the bot as a struct Position containing the row and column.
 * 
 * @see minimax, loadBoardStates, checkAndUpdateBestMove, writeBestMoveToFile
 */
struct Position findBestMove(int board[3][3]);

/**  
 * @brief Implements the Minimax algorithm to evaluate the best move for the bot.
 * 
 * The function recursively evaluates all possible moves using the Minimax 
 * algorithm. It returns the best score for the current player (maximizer or 
 * minimizer) based on the game state. The algorithm chooses the optimal move 
 * for the bot and evaluates the game state at each depth. The depth is capped 
 * if Minimax Godmode is not enabled. If there are no moves left or the game 
 * is over, it returns the evaluation score.
 * 
 * @param board A 3x3 array representing the current Tic-Tac-Toe board.
 * @param depth The current depth in the game tree.
 * @param isMax Boolean flag indicating whether it is the maximizer's turn (bot) or the minimizer's turn (player).
 * 
 * @return The best score for the current move based on the evaluation function.
 * 
 * @see evaluate, isMovesLeft, max, min
 */
int minimax(int board[3][3], int depth, bool isMax);

/**  
 * @brief Evaluates the current board state to determine if there is a winner.
 * 
 * This function checks the Tic-Tac-Toe board for winning conditions, i.e., 
 * it checks rows, columns, and diagonals for three consecutive marks (either 
 * `BOT` or `PLAYER1`). It returns a score based on the result:
 * - +10 if the `BOT` wins.
 * - -10 if `PLAYER1` wins.
 * - 0 if there is no winner yet (no winner in rows, columns, or diagonals).
 * 
 * @param b A 3x3 array representing the Tic-Tac-Toe board.
 * 
 * @return The evaluation score: 
 * - +10 for a `BOT` win,
 * - -10 for a `PLAYER1` win,
 * - 0 if there is no winner.
 * 
 * @see BOT, PLAYER1
 */
int evaluate(int b[3][3]);

/**
 * @brief Checks if there are any moves left on the board.
 *
 * This function determines if there are any empty cells left on a 3x3 board.
 * It uses one of the following implementations based on compilation options:
 * - A standard C implementation when `DISABLE_ASM` is defined.
 * - Optimized assembly instructions for either AArch64 or x86 platforms.
 *
 * The choice of implementation depends on the platform and preprocessor directives.
 *
 * @param board A 3x3 array representing the board state. Each cell should contain:
 * - `EMPTY` (typically 0) if the cell is empty.
 * - Any non-zero value if the cell is occupied.
 * @return `true` if there are empty cells; otherwise, `false`.
 *
 * @note If `DISABLE_ASM` is defined, the function uses pure C logic.
 * @note For AArch64 platforms, the function uses assembly instructions for efficient scanning.
 * @note For x86 platforms, the function uses assembly instructions for efficient scanning.
 *
 * Example usage:
 * @code
 * int board[3][3] = {
 *     {1, 2, 0},
 *     {0, 1, 2},
 *     {2, 1, 0}
 * };
 * bool movesLeft = isMovesLeft(board);
 * // movesLeft will be true as there are empty cells (0s).
 * @endcode
 *
 */
bool isMovesLeft(int board[3][3]);

/**  
 * @brief Checks if the current board configuration exists in the lookup table and updates the best move.
 * 
 * This function compares the current board with previously saved board states in the `boardStates` array. 
 * If a matching board configuration is found, it updates the provided `bestMove` structure with the best 
 * move associated with that board state. The function returns true if a match is found and the move is updated, 
 * and false if no match is found in the lookup table.
 * 
 * @param board The current Tic Tac Toe board to check against the saved states.
 * @param bestMove A pointer to the `Position` structure where the best move will be stored if a match is found.
 * @param boardStates An array of `BoardState` structures containing previously saved board configurations and their best moves.
 * @param count The number of saved board states in the `boardStates` array.
 * 
 * @return `true` if a matching board configuration is found and the best move is updated, `false` otherwise.
 * 
 * @see BoardState, Position
 */
bool checkAndUpdateBestMove(int board[3][3], struct Position *bestMove, struct BoardState boardStates[], int count);

/**  
 * @brief Appends the current board state and the best move to a file.
 * 
 * This function writes the current Tic Tac Toe board state to a file, encoding the board as a sequence of 
 * characters where 'o' represents Player 1, 'x' represents the Bot, and 'b' represents an empty cell. 
 * After writing the board state, it appends the best move (row and column) for the current board to the same file.
 * 
 * @param board The current Tic Tac Toe board to write to the file.
 * @param bestMove The best move to be made, represented by its row and column indices.
 * 
 * @see Position, BoardState
 */
void writeBestMoveToFile(int board[3][3], struct Position bestMove);

/**  
 * @brief Loads board states and their best moves from a file.
 * 
 * This function attempts to open a file containing saved board states and the 
 * corresponding best move for each state. If the file does not exist, a new 
 * file is created. It reads the board configurations and the best move 
 * for each board, storing them in the provided `boardStates` array.
 * 
 * Each line in the file represents one board state. The board is stored as 
 * a 3x3 grid, where 'x' denotes the BOT's move, 'o' denotes PLAYER1's move, 
 * and empty spaces are represented as ' ' (empty). The best move for each 
 * board is also saved in the file.
 * 
 * @param boardStates An array of `BoardState` structures to store the loaded board states.
 * 
 * @return The number of boards loaded from the file. If the file does not exist, 
 *         it returns 0 and creates a new file.
 * 
 * @see BoardState, FILE_BESTMOV
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