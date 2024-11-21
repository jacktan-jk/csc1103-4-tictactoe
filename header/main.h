/**
 * @file main.h
 * @author jacktan-jk
 * @brief Header file for the Tic-Tac-Toe game logic.
 * @version 1.0
 * @date 2024-11-12
 * 
 * @copyright Copyright (c) 2024
 * 
 * This file contains function declarations, structure definitions, and constants 
 * for handling game logic, button interactions, and the bot's behavior.
 */

#ifndef MAIN_H // Start of include guard
#define MAIN_H
#include <gtk/gtk.h>

#include <macros.h>
#include <minimax.h>
#include <ml-naive-bayes.h>
#include <elapsedTime.h>

/*===============================================================================================
GLOBAL DECLARATION
===============================================================================================*/
/**  
 * @var int iPlayer1_score
 * @brief Global variable to track Player 1's score.
 * 
 * @var int iPlayer2_score
 * @brief Global variable to track Player 2's or Bot's (Minimax/ML) score.
 * 
 * @var int iTie_score
 * @brief Global variable to track the number of ties/draws.
 * 
 * @var int iBoard[3][3]
 * @brief Global 2D array representing the Tic-Tac-Toe game board.
 * 
 * @var int iWinPos[3][3]
 * @brief Global 2D array to track winning positions on the board.
 * 
 * @var int iGameState
 * @brief Global variable to track the current game state.
 * 
 * Game states:
 * - PLAY: The game is ongoing.
 * - TIE: The game ended in a tie.
 * - WIN: A player has won the game.
 * 
 * @var bool isPlayer1Turn
 * @brief Global flag indicating if it's Player 1's turn.
 * 
 * @var bool isMLAvail
 * @brief Global flag indicating if Machine Learning mode is available.
 * This is set to false if the ML data file is missing, disabling the ML game mode.
 * 
 * @var struct PlayerMode playerMode
 * @brief Global structure to track the current game mode.
 * 
 * Fields:
 * - txt: Text representation of the current mode (e.g., "2P", "MM", "ML").
 * - mode: Integer value representing the current game mode.
 * 
 * Player modes:
 * - MODE_2P: Player vs Player mode.
 * - MODE_MM: Minimax Bot mode.
 * - MODE_ML: Machine Learning Bot mode.
 * 
 * @var GtkWidget *btnGrid[3][3]
 * @brief Global 2D array of buttons corresponding to the game grid.
 */

/** 
 * @struct BtnPos
 * @brief Stores the position of a button in the game grid.
 * 
 * This structure contains an array `pos[2]` that holds the row and column of 
 * the button in the grid.
 */
typedef struct
{
    int pos[2]; /**< Array to hold row and column */
} BtnPos;

/** 
 * @struct PlayerMode
 * @brief Stores the current game mode and its textual representation.
 * 
 * This structure holds the mode of the game (e.g., player vs player, player vs bot) 
 * and a textual representation of the mode for display.
 */
struct PlayerMode
{
    char txt[2]; /**< Textual representation of the current game mode */
    int mode;    /**< Integer value representing the current game mode */
};

/** 
 * @brief Executes the bot's move based on the current game mode.
 * 
 * In MM mode:
 * - Performs a minimax move.
 * - 20% chance of the minimax randomly selects a position.
 * 
 * In ML mode, the bot uses machine learning to determine the best position.
 * 
 * The function also measures and logs the time taken for the minimax move.
 * 
 * @return SUCCESS if the bot's move was made successfully.
 * @see playerMode, isMLAvail, iBoard, findBestMove, getBestPosition, btnGrid
 */
int doBOTmove();

/** 
 * @brief Checks the current game board for a win or tie.
 * 
 * This function checks all possible win conditions:
 * - Diagonals
 * - Rows
 * - Columns
 * 
 * If there is a winning line, it marks the winning positions and returns WIN.
 * If there are no winning conditions and the board is full, it returns TIE.
 * If there are unclicked positions left, it returns PLAY.
 * 
 * @return WIN if there is a winner, TIE if the game is a tie, PLAY if the game is still ongoing.
 * @see iBoard, iWinPos
 */
int chkPlayerWin();

/** 
 * @brief Clears the game board and resets the player's turn.
 * 
 * This function is used to reset the game board for a new round. It clears the labels
 * on the buttons in the grid and resets the internal board state (iBoard) to 0. It also
 * sets the player turn back to player 1.
 * 
 * @details
 * - Sets all button labels in the `btnGrid` to an empty string.
 * - Resets all values in the `iBoard` array to 0, indicating no moves.
 * - Resets `isPlayer1Turn` to `true`, indicating it’s Player 1's turn.
 * 
 * @see iBoard
 * @see btnGrid
 * @see isPlayer1Turn
 */
void clearGrid();

/** 
 * @brief Updates the score display on the button.
 * 
 * This function updates the label on a score button to display the current scores
 * for Player 1, Player 2, and Ties. It changes the text formatting depending on
 * which player's turn it is, highlighting the active player.
 * 
 * @param data A gpointer (usually a button widget) that is used to update the label.
 * 
 * @details
 * - The function checks if it's Player 1's turn and updates the score display with a bold label for Player 1, or Player 2’s turn with Player 2's score in bold.
 * - The button text is updated using `gtk_button_set_label()`, and the label markup is updated using `gtk_label_set_markup()`.
 * - The score includes Player 1's score, Player 2's score, the tie count, and the current game mode (`playerMode.txt`).
 * 
 * @see iPlayer1_score, iTie_score, iPlayer2_score, playerMode
 */
void updateScoreBtn(gpointer data);

/** 
 * @brief Callback function for handling button clicks on the game grid.
 * 
 * This function handles the logic for a player’s move when a button in the game grid is clicked.
 * It updates the game state, checks for a winner or tie, and updates the score display. It also handles
 * player turns, Bot moves (if applicable), and resets the game board when the game state changes.
 * 
 * @param widget The GtkWidget that was clicked (the button in the grid).
 * @param data Additional data passed to the callback (usually the score display data).
 * 
 * @details
 * - If the game state is not `PLAY`, the game will be reset, and the score updated.
 * - If the clicked button already has a label, the function returns early (no action is taken).
 * - If the clicked button is empty, the move is recorded in the `iBoard` array (Player 1 or MM or ML).
 * - After each move, the game checks for a win or tie condition using `chkPlayerWin()`.
 * - If Player 1 or Player 2 wins, the score is updated, and the win condition is shown.
 * - If the game ends in a tie, the tie score is updated.
 * - If the game is in **2P** mode, turns alternate between Player 1 and Player 2.
 * - In **MM mode**, the Minimax will automatically make a move after Player 1’s turn.
 * - In **ML mode**, the dataset is re-read and initialized after the game ends.
 * 
 * @see iBoard, isPlayer1Turn, iPlayer1_score, iPlayer2_score, iTie_score
 * @see playerMode, updateScoreBtn, chkPlayerWin, doBOTmove, showWin
 * @see PLAY, TIE, WIN
 */
void on_btnGrid_clicked(GtkWidget *widget, gpointer data);

/** 
 * @brief Handles button click for score.
 * 
 * Toggles the player mode and updates the displayed score.
 * 
 * @param widget The widget that triggered the event.
 * @param data Additional data passed to the callback.
 * 
 * @see playerMode, isMLAvail, isPlayer1Turn, updateScoreBtn, clearGrid
 */
void on_btnScore_clicked(GtkWidget *widget, gpointer data);

/** 
 * @brief Clears the winning positions and resets the grid.
 * 
 * Iterates over the win positions and clears any displayed labels,
 * resetting the grid to its initial state.
 * 
 * @see iWinPos, btnGrid
 */
void showWin();

#endif // MAIN_H  // End of include guard