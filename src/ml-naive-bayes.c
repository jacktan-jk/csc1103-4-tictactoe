#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 958
#define TRAINING_DATA_SIZE 0.8 * DATA_SIZE
#define TESTING_DATA_SIZE  0.2 * DATA_SIZE
#define CLASSES 2

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
        case 'x': return 0;
        case 'o': return 1;
        case 'b': return 2;
        default: return -1;
    }
}

void calculateProbabilities(int dataset_size) {
    // Calculate class probability
    positiveClassProbability =  (double) positive_count/dataset_size;
    negativeClassProbability = (double) negative_count/dataset_size;
    printf("Positive Class Probability: %lf\n", positiveClassProbability);
    printf("Negative Class Probability: %lf\n", negativeClassProbability);

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
                    printf("Probability of %c (positive) at grid(%d,%d): No positive outcomes\n", move, row, col);
                    printf("Probability of %c (negative) at grid(%d,%d): %lf\n", move, row, col, negativeProbability);
                }
                else if (negative_count == 0) {
                    printf("Probability of %c (positive) at grid(%d,%d): %lf\n", move, row, col, positiveProbability);
                    printf("Probability of %c (negative) at grid(%d,%d): No negative outcomes\n", move, row, col);
                }
                else {
                    printf("Probability of %c (positive) at grid(%d,%d): %lf\n", move, row, col, positiveProbability);
                    printf("Probability of %c (negative) at grid(%d,%d): %lf\n", move, row, col, negativeProbability);
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
    printf("\nPositive Probability: %lf\n", positiveProbability);
    printf("Negative Probability: %lf\n", negativeProbability);

    if (positiveProbability > negativeProbability) {
        printf("Predicted Outcome: Positive\n");
    } 
    else if (positiveProbability == 0 || negativeProbability == 0) {
        printf("Unable to predict outcome based on available data.");
    }
    else {
        printf("Predicted Outcome: Negative\n");
    }
}

// Get move and position with highest probability for bot
char getBestPosition(char grid[3][3], char player) {

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
            if (grid[row][col] == 'b') {
                // Calculate probability for X or O to determine best move for bot
                for (int moveIndex = 0; moveIndex < 2; moveIndex++) { 
                    char move;
                    if (moveIndex == 0) {
                        move = 'x';
                    } 
                    else {
                        move = 'o';
                    }

                    double moveProbability;
                    
                    if (bot == 'x') {
                        // Calculate probability for move 'x'
                        if (bot_count > 0) {
                            moveProbability = (double) botMoveCount[row][col][0] / bot_count;
                        } else {
                            moveProbability = 0.0;
                        }
                    } else {
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
    }

    // Return best position
    if (bestRow != -1 && bestCol != -1) {
        grid[bestRow][bestCol] = bestMove;
        printf("\nBest move: %c at grid (%d, %d) with probability: %lf\n", bestMove, bestRow, bestCol, highestProbability);
        return grid[bestRow][bestCol];
    } else {
        printf("\nNo valid move found.\n");
        return 'b'; // Indicate no valid move found
    }
}

int main() {
    //Testing gameboard for getBestPosition
    char gameBoard[3][3] = {{'x', 'x', 'o'}, {'b', 'o', 'x'}, {'b', 'b', 'b'}};
    getBestPosition(gameBoard, 'o');
    return 0;
}