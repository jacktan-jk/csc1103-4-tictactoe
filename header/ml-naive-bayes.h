/**
 * @file ml-naive-bayes.h
 * @author jacktan-jk
 * @brief Header file for Naive Bayes classifier functions for Tic-Tac-Toe outcome prediction.
 * @version 1.0
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2024
 * 
 * This file contains function declarations, structure definitions, and global variables for 
 * implementing the Naive Bayes classification model for predicting outcomes in a Tic-Tac-Toe game. 
 * It includes functions for initializing training data, calculating probabilities, 
 * predicting outcomes based on the trained model, calculating error rates, and updating the 
 * confusion matrix. The model is trained using a dataset of game board states and outcomes, 
 * and is used to predict the outcome of new game states.
 */

#ifndef ML_NAIVE_BAYES_H
#define ML_NAIVE_BAYES_H

#include <macros.h>
#include <importData.h>

#define TRAINING_DATA_SIZE 0.8 * DATA_SIZE     /**< Size of the training dataset (80%) */
#define TESTING_DATA_SIZE 0.2 * DATA_SIZE      /**< Size of the testing dataset (20%) */
#define CLASSES 2                              /**< Number of possible outcome classes (positive/negative) */

/**
 * @brief Assign an index for each move ("x", "o", or "b").
 * 
 * @param move The move character ('x', 'o', or 'b').
 * @return int The index representing the move (BOT, PLAYER1, EMPTY), or -1 if invalid.
 */
int assignMoveIndex(char move);

/**
 * @brief Calculate the class and conditional probabilities using the training dataset.
 * 
 * @param dataset_size The size of the dataset used to calculate probabilities.
 */
void calculateProbabilities(int dataset_size);

/**
 * @brief Reset all training data and statistics to their initial state.
 */
void resetTrainingData();

/**
 * @brief Initialize the training data by reading it from the dataset.
 * 
 * @return int Success or failure code (SUCCESS or ERROR).
 */
int initData();

/**
 * @brief Predict the outcome (positive/negative) of a given board state.
 * 
 * @param board The current Tic-Tac-Toe board.
 * @return int The predicted outcome (1 for positive, 0 for negative, -1 for error).
 */
int predictOutcome(struct Dataset board);

/**
 * @brief Calculate the training errors by comparing the predicted outcomes with actual outcomes.
 */
void calcTrainErrors();

/**
 * @brief Calculate the confusion matrix and error probabilities for the testing dataset.
 */
void calcConfusionMatrix();

/**
 * @brief Get the best move and position for the bot based on the highest probability.
 * 
 * @param grid The current Tic-Tac-Toe board grid.
 * @param player The current player ('x' or 'o').
 * @return struct Position The best move position for the bot.
 */
struct Position getBestPosition(int grid[3][3], char player);

/**
 * @brief Get the truth value of the outcome ('positive' or 'negative') from a string.
 * 
 * @param str1 The outcome string.
 * @return int The truth value (1 for positive, 0 for negative).
 */
int getTruthValue(char *str1);

/**
 * @brief Assign a value to the confusion matrix based on actual and predicted outcomes.
 * 
 * @param actual The actual outcome (positive/negative).
 * @param predicted The predicted outcome (positive/negative).
 */
void assignCMValue(int actual, int predicted);

/**
 * @brief Debug function to print the contents of the dataset.
 * 
 * @param data The dataset to be debugged.
 * @param len The length of the dataset.
 */
void debugDataset(struct Dataset *data, int len);

#endif // ML_NAIVE_BAYES_H
