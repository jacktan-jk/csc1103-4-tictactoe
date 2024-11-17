#include <ml-naive-bayes.h>
#include <math.h>

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
int positive_count = 0;
int negative_count = 0;

int cM[4] = {0, 0, 0, 0};
int positiveMoveCount[3][3][3];
int negativeMoveCount[3][3][3];

int test_PredictedErrors = 0;
int train_PredictedErrors = 0;

int predicted;
int actual;

double positiveClassProbability;
double negativeClassProbability;
double probabilityErrors;

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
int assignMoveIndex(char move) //converts char to int value for easier calculation
{
    switch (move)
    {
        case 'x':
            return BOT;
        case 'o':
            return PLAYER1;
        case 'b':
            return EMPTY;
        default: //guard case if input is invalid
            return ERROR;
    }
}

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
void calculateProbabilities(int dataset_size)
{
    // Calculate class probability
    positiveClassProbability = (double)positive_count / dataset_size;
    negativeClassProbability = (double)negative_count / dataset_size;
    PRINT_DEBUG("Positive Class Probability: %lf\n", positiveClassProbability);
    PRINT_DEBUG("Negative Class Probability: %lf\n", negativeClassProbability);

    // Calculate conditional probability with laplace smoothing
    int laplace_smoothing = 1;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            for (int moveIndex = 0; moveIndex < 3; moveIndex++)
            {
                char move;
                if (moveIndex == 0) //convert
                {
                    move = 'x';
                }
                else if (moveIndex == 1)
                {
                    move = 'o';
                }
                else
                {
                    move = 'b';
                }


                double positiveProbability = (double)(positiveMoveCount[row][col][moveIndex] + laplace_smoothing) / (positive_count + 3 * laplace_smoothing);
                double negativeProbability = (double)(negativeMoveCount[row][col][moveIndex] + laplace_smoothing) / (negative_count + 3 * laplace_smoothing);
                if (positive_count == 0)
                {
                    PRINT_DEBUG("Probability of %c (positive) at grid(%d,%d): No positive outcomes\n", move, row, col);
                    PRINT_DEBUG("Probability of %c (negative) at grid(%d,%d): %lf\n", move, row, col, negativeProbability);
                }
                else if (negative_count == 0)
                {
                    PRINT_DEBUG("Probability of %c (positive) at grid(%d,%d): %lf\n", move, row, col, positiveProbability);
                    PRINT_DEBUG("Probability of %c (negative) at grid(%d,%d): No negative outcomes\n", move, row, col);
                }
                else
                {
                    PRINT_DEBUG("Probability of %c (positive) at grid(%d,%d): %lf\n", move, row, col, positiveProbability);
                    PRINT_DEBUG("Probability of %c (negative) at grid(%d,%d): %lf\n", move, row, col, negativeProbability);
                }
            }
        }
    }
}

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
int predictOutcome(struct Dataset board)
{
    double positiveProbability = positiveClassProbability;
    double negativeProbability = negativeClassProbability;

    // required as 0*anything = 0
    if (positiveProbability == 0)
    {
        positiveProbability = 1;
    }
    if (negativeProbability == 0)
    {
        negativeProbability = 1;
    }

    //loops through board grid and sums up probability for each grid
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            int moveIndex = assignMoveIndex(board.grid[row][col]);
            if (moveIndex != -1)
            {
                // PRINT_DEBUG("\nPC_%d, NC_%d, pMC_%d, nMC_%d",positive_count,negative_count,positiveMoveCount[row][col][moveIndex],negativeMoveCount[row][col][moveIndex]);
                if (positive_count > 0)
                {
                    positiveProbability *= (double)positiveMoveCount[row][col][moveIndex] / (double)positive_count;
                }

                if (negative_count > 0)
                {
                    negativeProbability *= (double)negativeMoveCount[row][col][moveIndex] / (double)negative_count;
                }
            }
        }
    }

    // guard cases if either negativeProbability is unset
    if (positiveProbability == 1)
    {
        positiveProbability = 0;
    }
    if (negativeProbability == 1)
    {
        negativeProbability = 0;
    }

    // Output probabilities for debugging
    // PRINT_DEBUG("\nPositive: %lf, Negative: %lf Probability: \n", positiveProbability, negativeProbability);

    //returns a value based on condition
    if (positiveProbability > negativeProbability)
    {
        // PRINT_DEBUG("Predicted Outcome: Positive\n");
        return 1;
    }
    else if (positiveProbability == 0 || negativeProbability == 0)
    {
        // PRINT_DEBUG("Unable to predict outcome based on available data.");
        return -1;
    }
    else
    {
        // PRINT_DEBUG("Predicted Outcome: Negative\n");
        return 0;
    }
}


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
struct Position getBestPosition(int grid[3][3], char player)
{
    // Determine whether bot is X or O depending on current player
    char bot = (player == 'x' ? 'o' : 'x');
    char bestMove = 'b';
    int bestRow = -1;
    int bestCol = -1;
    double highestProbability = 0.0;

    int bot_count;
    int(*botMoveCount)[3][3];

    // Use positive or negative count for calculating probability depending on whether bot is X or O
    // Note that for the dataset, negative outcome is for X, meaning the position of O in negative outcomes are good for the bot playing as O
    if (bot == 'x')
    {
        bot_count = positive_count;
        botMoveCount = positiveMoveCount;
    }
    else
    {
        bot_count = negative_count;
        botMoveCount = negativeMoveCount;
    }

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            // If the grid position is empty
            if (grid[row][col] != EMPTY)
            {
                continue;
            }

            // Calculate probability for X or O to determine best move for bot
            for (int moveIndex = 0; moveIndex < 2; moveIndex++)
            {
                double moveProbability;

                if (bot == 'x')
                {
                    // Calculate probability for move 'x'
                    if (bot_count > 0)
                    {
                        moveProbability = (double)botMoveCount[row][col][0] / bot_count;
                    }
                    else
                    {
                        moveProbability = 0.0;
                    }
                }
                else
                {
                    // Calculate probability for move 'o'
                    if (bot_count > 0)
                    {
                        moveProbability = (double)botMoveCount[row][col][1] / bot_count;
                    }
                    else
                    {
                        moveProbability = 0.0;
                    }
                }

                // Update best move and position for bot if it has higher probability
                if (moveProbability > highestProbability)
                {
                    highestProbability = moveProbability;
                    bestMove = bot;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }
    }

    // Return best position
    if (bestRow != ERROR && bestCol != ERROR)
    {
        grid[bestRow][bestCol] = bestMove;
        PRINT_DEBUG("Best move: %c at grid (%d, %d) with probability: %lf\n", bestMove, bestRow, bestCol, highestProbability);
        return (struct Position){bestRow, bestCol};
    }
    else
    {
        PRINT_DEBUG("\nNo valid move found.\n");
        return (struct Position){ERROR, ERROR}; // Indicate no valid move found
    }
}

/**
 * @brief Resets the training data and associated statistics for a fresh training cycle.
 *
 * This function resets all relevant variables used in the machine learning model's training process.
 * It clears the outcome counts, resets the move count arrays for each grid position, and reinitializes the confusion matrix.
 * Additionally, it clears the prediction error counters, ensuring that the model starts with a clean state.
 *
 * @see positive_count, negative_count, positiveMoveCount, negativeMoveCount, cM, test_PredictedErrors, train_PredictedErrors
 */
void resetTrainingData() {
    // Reset outcome counts
    positive_count = 0;
    negative_count = 0;

    // Reset move count arrays for each grid position
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            for (int moveIndex = 0; moveIndex < 3; moveIndex++) {
                positiveMoveCount[row][col][moveIndex] = 0;
                negativeMoveCount[row][col][moveIndex] = 0;
            }
        }
    }

    // Reset the confusion matrix counters
    cM[0] = 0; // True positive
    cM[1] = 0; // False negative
    cM[2] = 0; // False positive
    cM[3] = 0; // True negative

    // Reset prediction errors
    test_PredictedErrors = 0;
    train_PredictedErrors = 0;
}

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
int initData()
{
    resetTrainingData();
    int retVal = SUCCESS;

    doGetTrainingData:
    struct Dataset *trainingData = NULL;      // Initialize pointer
    int len = getTrainingData(&trainingData); // Pass address of pointer

    if (len <= 0)
    {
        retVal = readDataset(RES_PATH "" DATA_PATH, true);
        if (retVal != SUCCESS)
        {
            return retVal;
        }
        goto doGetTrainingData; //loops until training data is set
    }

    //loops through train dataset for ml training
    for (int i = 0; i < len; i++)
    {
        // Get outcome class count for each position
        if (strcmp(trainingData[i].outcome, "positive") == 0)
        {
            positive_count++;
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    int moveIndex = assignMoveIndex(trainingData[i].grid[row][col]);
                    if (moveIndex != -1)
                    {
                        positiveMoveCount[row][col][moveIndex]++;
                    }
                }
            }
        }
        else if (strcmp(trainingData[i].outcome, "negative") == 0)
        {
            negative_count++;
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    int moveIndex = assignMoveIndex(trainingData[i].grid[row][col]);
                    if (moveIndex != -1)
                    {
                        negativeMoveCount[row][col][moveIndex]++;
                    }
                }
            }
        }
    }
    calcTrainErrors();
    calcConfusionMatrix();
    return SUCCESS;
}

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
void assignCMValue(int actual, int predicted)
{
    // PRINT_DEBUG("\nactual_%i, predicted_%i\n",actual,predicted);

    if (actual == ERROR || predicted == ERROR)
    {
        PRINT_DEBUG("ERROR either value is -1. actual: %d predicted: %d", actual, predicted);
    }

    if (actual == 1)
    {
        if (predicted == 1)
        {
            cM[0] += 1; // True positive
        }
        else
        {
            cM[1] += 1; // False negative
        }
    }
    else
    {
        if (predicted == 1)
        {
            cM[2] += 1; // False positive
        }
        else
        {
            cM[3] += 1; // True negative
        }
    }
}

void calcConfusionMatrix()
{
    //Tests ml on test dataset and stores result in a confusion matrix

    struct Dataset *test = NULL;
    int len = getTestingData(&test);
    // PRINT_DEBUG("Test_Data length: %d\n", len);
    //loops through testing dataset
    if (len > 0)
    { // Ensure len is valid before accessing test
        for (int i = 0; i < len; i++)
        {
            actual = getTruthValue(test[i].outcome); //converts char* to int for comparison
            predicted = predictOutcome(test[i]);

            // checks and updates total errors for test dataset
            if (actual != predicted)
            {
                test_PredictedErrors += 1;
            }

            //sets value based on actual vs predicted
            assignCMValue(actual, predicted);
        }
    }

    double i = TESTING_DATA_SIZE;                       // assign macro to double as you cant cast macros
    probabilityErrors = (1 / i) * test_PredictedErrors; // round to 2dp? not in spec though

    printf("For testing dataset: %d errors, %lf probability of error.\n", test_PredictedErrors, probabilityErrors);
    printf("TP: %d, FN: %d, FP: %d, TN: %d\n", cM[0], cM[1], cM[2], cM[3]);
}

/**
 * @brief Calculates the confusion matrix and error probability for the testing dataset.
 *
 * This function evaluates the model's performance by calculating the confusion matrix based on actual and predicted outcomes.
 * It iterates through the testing data, compares actual outcomes with predicted ones, and updates the confusion matrix values.
 * The number of prediction errors and the probability of error are also computed.
 *
 * @see cM, test_PredictedErrors, probabilityErrors, getTruthValue, predictOutcome
 */
int getTruthValue(char *str1) //returns an integer value based on input
{
    if (strcmp(str1, "positive") == 0)
    {
        return 1;
    }
    else if (strcmp(str1, "negative") == 0)
    {
        return 0;
    }
    else
    {
        //guard case if inputs are neither "positive" nor "negative"
        PRINT_DEBUG("ERROR: Not truth value: %p", str1);
        return -1;
    }
}

/**
 * @brief Calculates the training errors and the probability of error.
 *
 * This function evaluates the model's performance on the training dataset by comparing predicted outcomes with actual ones.
 * It updates the count of prediction errors and computes the probability of error based on the number of errors and the size of the training dataset.
 *
 * @see train_PredictedErrors, probabilityErrors, getTruthValue, predictOutcome
 */
void calcTrainErrors()
{
    struct Dataset *test = NULL;      // Initialize pointer
    int len = getTrainingData(&test); // Pass address of pointer
    // debugDataset(test,len);

    if (len > 0)
    { // Ensure len is valid before accessing test
        for (int i = 0; i < len; i++)
        {
            predicted = predictOutcome(test[i]);
            actual = getTruthValue(test[i].outcome);
            // PRINT_DEBUG("Actual dataset outcome: %s, Dataset outcome: %d, Predicted outcome: %d\n", test[i].outcome, actual, predicted);
            // checks and updates total errors for train dataset
            if (actual != predicted)
            {
                train_PredictedErrors += 1;
            }
        }
    }

    double i = TRAINING_DATA_SIZE;                       // assign macro to double var as macros cant be cast
    probabilityErrors = (1 / i) * train_PredictedErrors; // round to 2dp? not in spec though

    printf("\nFor training dataset: %d errors, %lf probability of error.\n", train_PredictedErrors, probabilityErrors);
}

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

void debugDataset(struct Dataset *data, int len)
{
    PRINT_DEBUG("%d\n", len);
    if (len > 0)
    { // Ensure len is valid before accessing test
        for (int i = 0; i < len; i++)
        {
            PRINT_DEBUG("%d ", i);
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    PRINT_DEBUG("%c,", data->grid[j][k]);
                }
            }
            PRINT_DEBUG("%s\n", data->outcome);
        }
    }
}
