/**
 * @file macros.h
 * @author jacktan-jk
 * @brief Header file containing macros, constants, and structure definitions for the Tic-Tac-Toe game.
 * @version 1.0
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2024
 * 
 * This file defines various constants for game modes, player identifiers, error codes, and debugging options.
 * It also includes the `Position` structure for grid positions in the game.
 * 
 */

#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// Constants for game states and player identifiers
#define PLAY 0x69        /**< Player move */
#define TIE 0xFF         /**< Tie state */
#define WIN 0xAA         /**< Winning state */
#define SUCCESS 0        /**< Success */
#define ERROR -1         /**< Error */
#define BAD_PARAM -5     /**< Bad parameter error */

// Game modes
#define MODE_2P 0        /**< Two-player mode */
#define MODE_MM 1        /**< Minimax mode */
#define MODE_ML 2        /**< Machine Learning mode */

// Player identifiers
#define EMPTY 0          /**< Empty cell */
#define PLAYER1 1        /**< Player 1 */
#define BOT 2            /**< Bot (Player 2) */

// Data constants
#define DATA_SIZE 958    /**< Dataset size */
#define CLASSES 2        /**< Number of outcome classes */

// Debugging and configuration options
#define DEBUG 1          /**< Enable debug messages */
#define MINIMAX_GODMODE 0/**< Minimax god mode toggle */
#define DISABLE_LOOKUP  0/**< Disable Minimax lookup table*/
#define DISABLE_ELAPSED 0/**< Disable Elapsed time function*/
#define DISABLE_ASM     0/**< Disable ASM functions*/

#if DEBUG
#define PRINT_DEBUG(...) printf(__VA_ARGS__);
#else
#define PRINT_DEBUG(...)
#endif

/**
 * @struct Position
 * @brief Represents a position on the Tic-Tac-Toe grid.
 * @var Position::row Row index (0-2)
 * @var Position::col Column index (0-2)
 */
struct Position
{
    int row; /**< Row index (0-2) */
    int col; /**< Column index (0-2) */
};

#endif // MACROS_H
