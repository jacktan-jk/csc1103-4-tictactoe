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
 * @brief Makes the bot perform a move based on the current game mode.
 * 
 * If in player vs player mode, the bot makes a strategic move, while in ML mode 
 * it selects the best move based on available data.
 * 
 * @return SUCCESS if the move is made successfully.
 * @see iBoard, BOT, findBestMove, getBestPosition
 */
int doBOTmove();

/** 
 * @brief Checks if a player has won the game.
 * 
 * This function checks all rows, columns, and diagonals for a winning condition.
 * If a player wins, the win positions are marked and `WIN` is returned. If the 
 * board is full and no player has won, `TIE` is returned.
 * 
 * @return WIN if a player has won, PLAY if the game is ongoing, TIE if it's a draw.
 * @see iBoard, iWinPos
 */
int chkPlayerWin();

/** 
 * @brief Clears the game buttons to reset the game grid.
 * 
 * This function resets the button labels and grid for a new game.
 * 
 * @see btnGrid
 */
void clearBtn();

/** 
 * @brief Updates the score display on the score button.
 * 
 * This function updates the score button label based on the current game scores 
 * and player mode.
 * 
 * @param data Additional data passed to the callback.
 * @see playerMode, isPlayer1Turn, iPlayer1_score, iPlayer2_score
 */
void updateScoreBtn(gpointer data);

/** 
 * @brief Handles the click event for a button in the game grid.
 * 
 * This function handles user interactions with the game grid and updates 
 * the game state based on the player's move.
 * 
 * @param widget The widget that triggered the event.
 * @param data Additional data passed to the callback.
 * @see iBoard, isPlayer1Turn, updateScoreBtn
 */
void on_btnGrid_clicked(GtkWidget *widget, gpointer data);

/** 
 * @brief Handles the click event for the score button.
 * 
 * This function toggles the game mode and updates the displayed score when 
 * the score button is clicked.
 * 
 * @param widget The widget that triggered the event.
 * @param data Additional data passed to the callback.
 * @see playerMode, isMLAvail, isPlayer1Turn, updateScoreBtn, clearBtn
 */
void on_btnScore_clicked(GtkWidget *widget, gpointer data);

/** 
 * @brief Displays the winning positions on the game grid.
 * 
 * This function updates the game grid to show the positions of the winning 
 * combination, if any, and clears the board for a new game.
 * 
 * @see iWinPos, iBoard
 */
void showWin();

#endif // MAIN_H  // End of include guard