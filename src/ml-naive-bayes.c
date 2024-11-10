#include <ml-naive-bayes.h>
#include <math.h>

// Counters for positive and negative outcomes
int positive_count = 0;
int negative_count = 0;

// Class probability for positive or negative outcomes
double positiveClassProbability;
double negativeClassProbability;

// Counters for each move "x", "o" or "b" with their board position and their respective outcomes
int positiveMoveCount[3][3][3];
int negativeMoveCount[3][3][3];

// Assign an index for each move "x", "o" or "b"
int assignMoveIndex(char move) {
    switch(move) {
        case 'x': return BOT;
        case 'o': return PLAYER1;
        case 'b': return EMPTY;
        default: return -1;
    }
}

int cM[4] = {0,0,0,0};
int test_PredictedErrors = 0;
int train_PredictedErrors = 0;
double probabilityErrors;
int predicted;
int actual;

void calculateProbabilities(int dataset_size) {
    // Calculate class probability
    positiveClassProbability =  (double) positive_count/dataset_size;
    negativeClassProbability = (double) negative_count/dataset_size;
    PRINT_DEBUG("Positive Class Probability: %lf\n", positiveClassProbability);
    PRINT_DEBUG("Negative Class Probability: %lf\n", negativeClassProbability);

    // Calculate conditional probability with laplace smoothing
    int laplace_smoothing = 1;  
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            for (int moveIndex = 0; moveIndex < 3; moveIndex++) {
                char move;
                if (moveIndex == 0) {
                    move = 'x';
                } 
                else if (moveIndex == 1) {
                    move = 'o';
                } 
                else {
                    move = 'b';
                }
                double positiveProbability = (double) (positiveMoveCount[row][col][moveIndex] + laplace_smoothing)/(positive_count + 3 * laplace_smoothing);
                double negativeProbability = (double) (negativeMoveCount[row][col][moveIndex] + laplace_smoothing)/(negative_count + 3 * laplace_smoothing);
                if (positive_count == 0) {
                    PRINT_DEBUG("Probability of %c (positive) at grid(%d,%d): No positive outcomes\n", move, row, col);
                    PRINT_DEBUG("Probability of %c (negative) at grid(%d,%d): %lf\n", move, row, col, negativeProbability);
                }
                else if (negative_count == 0) {
                    PRINT_DEBUG("Probability of %c (positive) at grid(%d,%d): %lf\n", move, row, col, positiveProbability);
                    PRINT_DEBUG("Probability of %c (negative) at grid(%d,%d): No negative outcomes\n", move, row, col);
                }
                else {
                    PRINT_DEBUG("Probability of %c (positive) at grid(%d,%d): %lf\n", move, row, col, positiveProbability);
                    PRINT_DEBUG("Probability of %c (negative) at grid(%d,%d): %lf\n", move, row, col, negativeProbability);
                }
            }
        }
    }
}

int predictOutcome(struct Dataset board) {
    double positiveProbability = positiveClassProbability;
    double negativeProbability = negativeClassProbability;

    //required as 0*anything = 0
    if(positiveProbability == 0)
    {
        positiveProbability = 1;
    }
    if (negativeProbability == 0)
    {
        negativeProbability = 1;
    }

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            int moveIndex = assignMoveIndex(board.grid[row][col]);
            if (moveIndex != -1)
            {
                //PRINT_DEBUG("\nPC_%d, NC_%d, pMC_%d, nMC_%d",positive_count,negative_count,positiveMoveCount[row][col][moveIndex],negativeMoveCount[row][col][moveIndex]);
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

    //guard cases if either negativeProbability is unset
    if (positiveProbability == 1)
    {
        positiveProbability = 0;
    }
    if (negativeProbability == 1)
    {
        negativeProbability = 0;
    }

    // Output probabilities for debugging
    //PRINT_DEBUG("\nPositive: %lf, Negative: %lf Probability: \n", positiveProbability, negativeProbability);

    if (positiveProbability > negativeProbability) {
        //PRINT_DEBUG("Predicted Outcome: Positive\n");
        return 1;
    } 
    else if (positiveProbability == 0 || negativeProbability == 0) {
        //PRINT_DEBUG("Unable to predict outcome based on available data.");
        return -1;
    }
    else {
        //PRINT_DEBUG("Predicted Outcome: Negative\n");
        return 0;
    }
}

// Get move and position with highest probability for bot
struct Position getBestPosition(int grid[3][3], char player) {
    // Determine whether bot is X or O depending on current player
    char bot = (player == 'x' ? 'o' : 'x');    
    char bestMove = 'b';
    int bestRow = -1;
    int bestCol = -1;
    double highestProbability = 0.0;

    int bot_count;
    int (*botMoveCount)[3][3];
 
    // Use positive or negative count for calculating probability depending on whether bot is X or O
    // Note that for the dataset, negative outcome is for X, meaning the position of O in negative outcomes are good for the bot playing as O
    if (bot == 'x') {
        bot_count = positive_count;
        botMoveCount = positiveMoveCount;

    } 
    else {
        bot_count = negative_count;
        botMoveCount = negativeMoveCount;
    }

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
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
                    if (bot_count > 0) {
                        moveProbability = (double) botMoveCount[row][col][0] / bot_count;
                    } else {
                        moveProbability = 0.0;
                    }
                } 
                else
                {
                    // Calculate probability for move 'o'
                    if (bot_count > 0) {
                        moveProbability = (double) botMoveCount[row][col][1] / bot_count;
                    } else {
                        moveProbability = 0.0;
                    }
                }

                // Update best move and position for bot if it has higher probability
                if (moveProbability > highestProbability) {
                    highestProbability = moveProbability;
                    bestMove = bot;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }
    }

    // Return best position
    if (bestRow != ERROR && bestCol != ERROR) {
        grid[bestRow][bestCol] = bestMove;    
        PRINT_DEBUG("Best move: %c at grid (%d, %d) with probability: %lf\n", bestMove, bestRow, bestCol, highestProbability);
        return (struct Position){bestRow,bestCol};
    } else {
        PRINT_DEBUG("\nNo valid move found.\n");
        return (struct Position){ERROR,ERROR}; // Indicate no valid move found
    }
}

int initData()
{
    int retVal = SUCCESS;

doGetTrainingData:
    struct Dataset *trainingData = NULL; // Initialize pointer
    int len = getTrainingData(&trainingData); // Pass address of pointer

    if(len <= 0)
     {
        retVal = readDataset(RES_PATH""DATA_PATH, true);
        if (retVal != SUCCESS)
        {
            return retVal;
        }
        goto doGetTrainingData;
    }

    for (int i = 0; i < len; i++) 
    {
        // Get outcome class count
        if (strcmp(trainingData[i].outcome, "positive") == 0) 
        {
            positive_count++;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    int moveIndex = assignMoveIndex(trainingData[i].grid[row][col]);
                    if (moveIndex != -1) {
                        positiveMoveCount[row][col][moveIndex]++;
                    }
                }
            }
        }
        else if (strcmp(trainingData[i].outcome, "negative") == 0) 
        {
            negative_count++;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    int moveIndex = assignMoveIndex(trainingData[i].grid[row][col]);
                    if (moveIndex != -1) {
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

void assignCMValue(int actual, int predicted)
{
    //PRINT_DEBUG("\nactual_%i, predicted_%i\n",actual,predicted);

    if(actual == -1 || predicted == -1)
    {
        PRINT_DEBUG("ERROR either value is -1. actual: %d predicted: %d", actual, predicted);
    }

    if (actual == 1)
    {
        if (predicted == 1)
        {
            cM[0] += 1; //True positive
        }
        else
        {
            cM[1] += 1; //False negative
        }
    }
    else
    {
        if (predicted == 1)
        {
            cM[2] += 1; //False positive
        }
        else
        {
            cM[3] += 1; //True negative
        }
    }
}

void calcConfusionMatrix()
{
    struct Dataset *test = NULL;
    int len = getTestingData(&test);
    //PRINT_DEBUG("Test_Data length: %d\n", len);
    if (len > 0) { // Ensure len is valid before accessing test
        for (int i = 0; i < len; i++)
        {
            /*PRINT_DEBUG("%d ", i);
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    PRINT_DEBUG("%c,", train[i].grid[j][k]);

                }
            }*/
            actual = getTruthValue(test[i].outcome);
            predicted = predictOutcome(test[i]);

            //probability of error calculation
            if (actual != predicted)
            {
                test_PredictedErrors += 1;
            }
            assignCMValue(actual,predicted);
        }
    }

    double i = TESTING_DATA_SIZE; //assign macro to double as you cant cast macros
    probabilityErrors = (1 / i)*test_PredictedErrors; //round to 2dp? not in spec though

    PRINT_DEBUG("For testing dataset: %d errors, %lf probability of error.\n", test_PredictedErrors,probabilityErrors);
    PRINT_DEBUG("TP: %d, FN: %d, FP: %d, TN: %d\n",cM[0],cM[1],cM[2],cM[3]);

/*    PRINT_DEBUG("%s\n", train[190].outcome);
    //assignCMValue(train[0].outcome, predicted);
    PRINT_DEBUG("A_outcome %s\n",train[0].outcome);*/
}

int getTruthValue(char* str1)
{
    if (strcmp(str1,"positive") == 0)
    {
        return 1;
    }
    else if (strcmp(str1,"negative") == 0)
    {
        return 0;
    }
    else
    {
        PRINT_DEBUG("ERROR: Not truth value: %p", str1);
        return -1;
    }
}

void calcTrainErrors()
{
    struct Dataset *test = NULL; // Initialize pointer
    int len = getTrainingData(&test); // Pass address of pointer
    //debugDataset(test,len);

    if (len > 0)
    { // Ensure len is valid before accessing test
        for (int i = 0; i < len; i++)
        {
            predicted = predictOutcome(test[i]);
            actual = getTruthValue(test[i].outcome);
            //PRINT_DEBUG("Actual dataset outcome: %s, Dataset outcome: %d, Predicted outcome: %d\n", test[i].outcome, actual, predicted);
            if (actual != predicted)
            {
                train_PredictedErrors += 1;
            }
        }
    }

    double i = TRAINING_DATA_SIZE; //assign macro to double var as macros cant be cast
    probabilityErrors = (1 / i)*train_PredictedErrors; //round to 2dp? not in spec though

    PRINT_DEBUG("\nFor training dataset: %d errors, %lf probability of error.\n", train_PredictedErrors, probabilityErrors);
}

void rbset()
{
    int retVal = SUCCESS;
    retVal = readDataset(RES_PATH""DATA_PATH, true);
}

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
                    PRINT_DEBUG("%c,", data -> grid[j][k]);
                }
            }
            PRINT_DEBUG("%s\n", data -> outcome);
        }
    }
}
