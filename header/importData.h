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
 * @brief Reads a dataset from a file and optionally randomizes entries for training and testing.
 * 
 * Opens a file to read each line as a Tic Tac Toe board state, populating a grid structure 
 * where 'x', 'o', and 'b' represent the Bot, Player 1, and empty cells, respectively. 
 * Each board state is followed by an outcome that is stored within the dataset. 
 * If `split` is true, entries are randomized using an array of unique indices for shuffling.
 * 
 * @param filename The name of the dataset file to read.
 * @param split Boolean indicating whether to randomize entries for dataset splitting.
 * @return int SUCCESS (0) if reading is successful, BAD_PARAM (-5) if the file cannot be opened, 
 *         or the return value of `splitFile()` if `split` is enabled.
 * 
 * @see getRandomNo, splitFile
 */
int readDataset(const char *filename, bool split);

/** 
 * @brief Splits the dataset into training and testing files with an 80-20 ratio.
 * 
 * This function separates the dataset into two parts: 80% for training and 20% for testing. 
 * The training portion is written to `trainingFile`, and the testing portion is written to 
 * `testingFile`. Each entry consists of a 3x3 grid representing the Tic Tac Toe board and 
 * the outcome of that board.
 * 
 * @return int SUCCESS (0) if both files are written successfully, BAD_PARAM (-5) if either 
 *         file cannot be opened.
 * 
 * @see data, trainingFile, testingFile
 */
int splitFile();

/** 
 * @brief Generates an array of unique random integers within the range of the dataset size.
 * 
 * This function populates an array with unique random integers between 0 and `DATA_SIZE - 1`. 
 * It ensures that each integer appears only once by checking a `check` array to track used 
 * indices. This can be used for randomizing the order of data for splitting purposes.
 * 
 * @param random Array to store the generated unique random integers.
 * 
 * @see DATA_SIZE
 */
void getRandomNo(int random[DATA_SIZE]);

/** 
 * @brief Retrieves the training data from a file and returns its length.
 * 
 * This function initializes the `data` array to zero for the length of the training set, reads 
 * the dataset from the specified `trainingFile`, and assigns the `data` pointer to the provided 
 * dataset pointer. Returns the length of the training data loaded.
 * 
 * @param d Pointer to a dataset pointer that will reference the loaded training data array.
 * 
 * @return The number of training entries loaded (i.e., `len_train`).
 * 
 * @see readDataset, trainingFile
 */
int getTrainingData(struct Dataset **d);

/** 
 * @brief Retrieves the testing data from a file and returns its length.
 * 
 * This function zeroes out the `data` array for the length of the testing set, reads the 
 * dataset from the specified `testingFile`, and assigns the `data` pointer to the provided 
 * dataset pointer. Returns the length of the testing data loaded.
 * 
 * @param d Pointer to a dataset pointer that will reference the loaded testing data array.
 * 
 * @return The number of testing entries loaded (i.e., `len_test`).
 * 
 * @see readDataset, testingFile
 */
int getTestingData(struct Dataset **d);

#endif // IMPORTDATA_H
