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
 * @var positive_count
 * @brief Counter for the number of positive outcomes in the training dataset.
 * 
 * @var negative_count
 * @brief Counter for the number of negative outcomes in the training dataset.
 * 
 * @var positiveClassProbability
 * @brief Probability of a positive outcome in the dataset.
 * 
 * @var negativeClassProbability
 * @brief Probability of a negative outcome in the dataset.
 * 
 * @var positiveMoveCount
 * @brief 3D array to count occurrences of each move for positive outcomes.
 * 
 * @var negativeMoveCount
 * @brief 3D array to count occurrences of each move for negative outcomes.
 * 
 * @var cM
 * @brief Confusion matrix for evaluating model performance.
 * 
 * @var test_PredictedErrors
 * @brief Counter for the number of errors in the testing dataset predictions.
 * 
 * @var train_PredictedErrors
 * @brief Counter for the number of errors in the training dataset predictions.
 * 
 * @var probabilityErrors
 * @brief Probability of error in the predictions, calculated from the testing dataset.
 * 
 * @var predicted
 * @brief The predicted outcome for the current dataset (1 for positive, 0 for negative).
 * 
 * @var actual
 * @brief The actual outcome for the current dataset (1 for positive, 0 for negative).
 */

/**  
 * @brief Assigns an index to each move ("x", "o", or "b").
 * 
 * This function maps the board move characters to their corresponding integer values:
 * - 'x' is mapped to the BOT.
 * - 'o' is mapped to PLAYER1.
 * - 'b' is mapped to EMPTY.
 * If the character does not match any of the valid moves, -1 is returned.
 * 
 * @param move The character representing the move ('x', 'o', or 'b').
 * @return int The integer corresponding to the move:
 *         - BOT for 'x',
 *         - PLAYER1 for 'o',
 *         - EMPTY for 'b',
 *         - ERROR for invalid input.
 * 
 * @see BOT, PLAYER1, EMPTY, ERROR
 */
int assignMoveIndex(char move);

/**  
 * @brief Calculates the probabilities for each class and conditional probabilities with Laplace smoothing.
 * 
 * This function calculates:
 * - The class probabilities for positive and negative outcomes.
 * - The conditional probabilities for each move ('x', 'o', 'b') at each position on the board,
 *   given the class (positive or negative) with Laplace smoothing applied.
 * 
 * The Laplace smoothing is used to prevent zero probabilities for moves that may not have been observed in the training data.
 * The resulting probabilities are printed for debugging purposes.
 * 
 * @param dataset_size The total number of samples in the dataset used for probability calculation.
 * 
 * @see positive_count, negative_count, positiveMoveCount, negativeMoveCount
 */
void calculateProbabilities(int dataset_size);

/**  
 * @brief Resets the training data and associated statistics for a fresh training cycle.
 * 
 * This function resets all relevant variables used in the machine learning model's training process.
 * It clears the outcome counts, resets the move count arrays for each grid position, and reinitializes the confusion matrix.
 * Additionally, it clears the prediction error counters, ensuring that the model starts with a clean state.
 * 
 * @see positive_count, negative_count, positiveMoveCount, negativeMoveCount, cM, test_PredictedErrors, train_PredictedErrors
 */
void resetTrainingData();

/**  
 * @brief Initializes the training data and model statistics.
 * 
 * This function resets the training data, then retrieves the training dataset for model training. 
 * It processes the dataset to count occurrences of positive and negative outcomes and updates the move counts for each grid position based on the data. 
 * Afterward, it calculates training errors and updates the confusion matrix.
 * 
 * If the initial dataset is empty, it attempts to load the data again.
 * 
 * @see resetTrainingData, getTrainingData, calcTrainErrors, calcConfusionMatrix
 */
int initData();

/**  
 * @brief Predicts the outcome of a given Tic Tac Toe board based on previously calculated probabilities.
 * 
 * This function calculates the probabilities of a positive (Player 1 wins) or negative (Bot wins) outcome
 * for a given board state by multiplying the conditional probabilities of each move in the grid with the
 * class probabilities. The prediction is made based on which outcome (positive or negative) has the higher probability.
 * 
 * If the calculated probabilities are zero, indicating that the outcome cannot be predicted with the available data,
 * the function returns -1.
 * 
 * @param board The current Tic Tac Toe board whose outcome needs to be predicted.
 * 
 * @return 1 if the predicted outcome is positive (Player 1 wins), 0 if negative (Bot wins), and -1 if the outcome cannot be predicted.
 * 
 * @see positiveClassProbability, negativeClassProbability, positiveMoveCount, negativeMoveCount, assignMoveIndex
 */
int predictOutcome(struct Dataset board);

/**  
 * @brief Calculates the training errors and the probability of error.
 * 
 * This function evaluates the model's performance on the training dataset by comparing predicted outcomes with actual ones. 
 * It updates the count of prediction errors and computes the probability of error based on the number of errors and the size of the training dataset.
 * 
 * @see train_PredictedErrors, probabilityErrors, getTruthValue, predictOutcome
 */
void calcTrainErrors();

/**  
 * @brief Calculates the confusion matrix and error probability for the testing dataset.
 * 
 * This function evaluates the model's performance by calculating the confusion matrix based on actual and predicted outcomes. 
 * It iterates through the testing data, compares actual outcomes with predicted ones, and updates the confusion matrix values. 
 * The number of prediction errors and the probability of error are also computed.
 * 
 * @see cM, test_PredictedErrors, probabilityErrors, getTruthValue, predictOutcome
 */
void calcConfusionMatrix();

/**  
 * @brief Determines the best position for the bot to make a move based on the highest probability.
 * 
 * This function evaluates all empty positions on the Tic Tac Toe grid and calculates the probability
 * of the bot winning (either as 'x' or 'o') using the pre-calculated move probabilities from the training data. 
 * The bot chooses the position with the highest probability of winning, where the move is either 'x' or 'o' 
 * depending on the current player. It returns the best position for the bot to make its move.
 * 
 * @param grid The current state of the Tic Tac Toe game board.
 * @param player The current player, either 'x' or 'o'.
 * 
 * @return A struct `Position` representing the row and column of the best move for the bot. If no valid move is found, it returns an error indicator.
 * 
 * @see positive_count, negative_count, positiveMoveCount, negativeMoveCount
 */
struct Position getBestPosition(int grid[3][3], char player);

/**
 * @brief Returns an integer value representing a truth value based on input.
 * 
 * This function converts a string input into a corresponding integer truth value. 
 * The input string is compared against "positive" and "negative" to determine the output:
 * - Returns `1` for "positive".
 * - Returns `0` for "negative".
 * - Returns `-1` for any other input, and logs an error message for invalid cases.
 * 
 * @param str1 A pointer to the input string to be evaluated.
 * 
 * @return int The corresponding truth value:
 * - `1` for "positive".
 * - `0` for "negative".
 * - `-1` for invalid inputs.
 * 
 * @see PRINT_DEBUG
 */
int getTruthValue(char *str1);

/**  
 * @brief Updates the confusion matrix based on actual and predicted outcomes.
 * 
 * This function updates the confusion matrix counters for true positives, false negatives, false positives, and true negatives. 
 * It checks the actual and predicted outcomes and increments the appropriate counter in the confusion matrix.
 * 
 * If either the actual or predicted value is ERROR, an error is logged.
 * 
 * @param actual The actual outcome value (1 for positive, 0 for negative).
 * @param predicted The predicted outcome value (1 for positive, 0 for negative).
 * 
 * @see cM, ERROR
 */
void assignCMValue(int actual, int predicted);

/**  
 * @brief Debug function to display dataset contents.
 * 
 * This function prints the details of the provided dataset, including the grid values and the corresponding outcomes. 
 * It is primarily used for debugging purposes and is not currently in use within the code.
 * 
 * @param data Pointer to the dataset to be printed.
 * @param len The length of the dataset (number of entries).
 * 
 * @see PRINT_DEBUG
 */
void debugDataset(struct Dataset *data, int len);

#endif // ML_NAIVE_BAYES_H
