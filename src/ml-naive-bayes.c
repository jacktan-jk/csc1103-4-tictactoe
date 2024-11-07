#include <ml-naive-bayes.h>

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

void predictOutcome(char grid[3][3]) {
    double positiveProbability = positiveClassProbability;
    double negativeProbability = negativeClassProbability;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int moveIndex = assignMoveIndex(grid[row][col]);
            
            if (moveIndex != -1) {
                if (positive_count > 0) {
                    positiveProbability *= (double)positiveMoveCount[row][col][moveIndex] / positive_count;
                }
                else if (negative_count > 0) {
                    negativeProbability *= (double)negativeMoveCount[row][col][moveIndex] / negative_count;
                }
            }
        }
    }
    
    // Output probabilities for debugging
    PRINT_DEBUG("\nPositive Probability: %lf\n", positiveProbability);
    PRINT_DEBUG("Negative Probability: %lf\n", negativeProbability);

    if (positiveProbability > negativeProbability) {
        PRINT_DEBUG("Predicted Outcome: Positive\n");
    } 
    else if (positiveProbability == 0 || negativeProbability == 0) {
        PRINT_DEBUG("Unable to predict outcome based on available data.");
    }
    else {
        PRINT_DEBUG("Predicted Outcome: Negative\n");
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
    return SUCCESS;
}