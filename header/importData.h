/** 
 * @file importdata.h
 * @author jacktan-jk
 * @brief Header file for handling dataset import and manipulation for Tic-Tac-Toe game data.
 * @version 1.0
 * @date 2024-11-12
 * 
 * This file contains function declarations and structures to read, split, and manage
 * Tic-Tac-Toe game data used for training and testing.
 */
#ifndef IMPORTDATA_H
#define IMPORTDATA_H

#include <macros.h>

// changed to 100 for testing. make sure to chg back
#define RES_PATH "./resources/"       /**< Path to resources directory */
#define DATA_PATH "tic-tac-toe.data"   /**< Name of the primary dataset file */
#define TRAIN_PATH "training-"         /**< Prefix for training data file */
#define TEST_PATH "testing-"           /**< Prefix for testing data file */

/** 
 * @struct Dataset
 * @brief Structure to hold a Tic-Tac-Toe board state and its outcome.
 * 
 * This structure represents the board state as a 3x3 grid and the outcome as a string.
 */
struct Dataset
{
    char grid[3][3];    /**< 3x3 grid representing the board state */
    char outcome[9];    /**< Outcome of the board state */
};

/** 
 * @brief Reads a dataset file and optionally splits data randomly.
 * 
 * @param filename Name of the file to read.
 * @param split Flag indicating whether to split data into training/testing.
 * @return Success or error code.
 */
int readDataset(const char *filename, bool split);

/** 
 * @brief Splits dataset into 80% training and 20% testing files.
 * 
 * @return Success or error code.
 */
int splitFile();

/** 
 * @brief Generates an array of unique random numbers within the dataset size.
 * 
 * @param random Array to store generated random numbers.
 */
void getRandomNo(int random[DATA_SIZE]);

/** 
 * @brief Retrieves the training data and its length.
 * 
 * @param[out] d Pointer to a dataset pointer that will reference the training data.
 * @return Number of entries in the training data.
 */
int getTrainingData(struct Dataset **d);

/** 
 * @brief Retrieves the testing data and its length.
 * 
 * @param[out] d Pointer to a dataset pointer that will reference the testing data.
 * @return Number of entries in the testing data.
 */
int getTestingData(struct Dataset **d);

#endif // IMPORTDATA_H
