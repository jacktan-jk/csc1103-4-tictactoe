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

int predictOutcome(struct Dataset board) {
    double positiveProbability = positiveClassProbability;
    double negativeProbability = negativeClassProbability;

    if(positiveProbability == 0){
        positiveProbability = 1;
    }

    if (negativeProbability == 0){
        negativeProbability = 1;
    }

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int moveIndex = assignMoveIndex(board.grid[row][col]);
            if (moveIndex != -1) {
                //printf("\nPC_%d, NC_%d, pMC_%d, nMC_%d",positive_count,negative_count,positiveMoveCount[row][col][moveIndex],negativeMoveCount[row][col][moveIndex]);
                if (positive_count > 0) {
                    positiveProbability *= (double)positiveMoveCount[row][col][moveIndex] / (double)positive_count;
                    //printf("\npP %lf", positiveProbability);
                }

                if (negative_count > 0) {
                    negativeProbability *= (double)negativeMoveCount[row][col][moveIndex] / (double)negative_count;
                    //printf("nP %lf", negativeProbability);
                }
            }
        }
    }

    if (positiveProbability == 1){
        positiveProbability = 0;
    }

    if (negativeProbability == 1){
        negativeProbability = 0;
    }

    //printf("\nPC_%d, NC_%d",positive_count,negative_count);
    
    // Output probabilities for debugging
    //printf("\nPositive Probability: %lf\n", positiveProbability);
    //printf("Negative Probability: %lf\n", negativeProbability);

    if (positiveProbability > negativeProbability) {
        //printf("Predicted Outcome: Positive\n");
        return 1;
    } 
    else if (positiveProbability == 0 || negativeProbability == 0) {
        //printf("Unable to predict outcome based on available data.");
        return -1;
    }
    else {
        //printf("Predicted Outcome: Negative\n");
        return 0;
    }
}

// Get move and position with highest probability for bot
struct Position getBestPosition(char grid[3][3], char player) {
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
            if (grid[row][col] != 'b') 
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
    if (bestRow != -1 && bestCol != -1) {
        grid[bestRow][bestCol] = bestMove;    
        printf("\nBest move: %c at grid (%d, %d) with probability: %lf\n", bestMove, bestRow, bestCol, highestProbability);
        return (struct Position){bestRow,bestCol};
    } else {
        printf("\nNo valid move found.\n");
        return (struct Position){ERROR,ERROR}; // Indicate no valid move found
    }
}

void initData(struct Dataset *data, int len)
{
    for (int i = 0; i < len; i++) 
    {
        // Get outcome class count
        if (strcmp(data[i].outcome, "positive") == 0) 
        {
            positive_count++;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    int moveIndex = assignMoveIndex(data[i].grid[row][col]);
                    if (moveIndex != -1) {
                        positiveMoveCount[row][col][moveIndex]++;
                    }
                }
            }
        }
        else if (strcmp(data[i].outcome, "negative") == 0) 
        {
            negative_count++;
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    int moveIndex = assignMoveIndex(data[i].grid[row][col]);
                    if (moveIndex != -1) {
                        negativeMoveCount[row][col][moveIndex]++;
                    }
                }
            }
        }
    }
}

void rbset(){
    int retVal = SUCCESS;
    retVal = readDataset(RES_PATH""DATA_PATH, true);
}

void cm_value(int actual, int predicted){
    //printf("\nactual_%i, predicted_%i\n",actual,predicted);

    if(actual == -1 || predicted == -1){
        printf("ERROR either value is -1");
    }

    if (actual == 1){
        if (predicted == 1){
            cM[0] += 1; //true positive
        }else{
            cM[1] += 1; //true negative
        }
    }else{
        if (predicted == 1){
            cM[2] += 1; //false positive
        }else{
            cM[3] += 1; //false negative
        }
    }
}



void calc_confusion_matrix(){
    struct Dataset *train = NULL;
    int lenTest = getTestingData(&train);
    //printf("test_%d\n", lenTest);
    if (lenTest > 0) { // Ensure len is valid before accessing test
        for (int i = 0; i < lenTest; i++) {
            /*printf("%d ", i);
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    printf("%c,", train[i].grid[j][k]);

                }
            }*/
            //printf("%s\n", train[i].outcome);
            if (strcmp(train[i].outcome,"positive") == 0){
                actual = 1;
            } else if (strcmp(train[i].outcome,"negative") == 0){
                actual = 0;
            } else {
                actual = -1;
            }
            predicted = predictOutcome(train[i]);
            //printf("actual: %i, predicted: %i\n",actual,predicted);

            //probability of error calculation
            if (actual != predicted){
                test_PredictedErrors += 1;
            }

            cm_value(actual,predicted);
        }
    }

    double i = TESTING_DATA_SIZE; //assign macro to double as you cant cast macros
    probabilityErrors = (1 / i)*test_PredictedErrors; //round to 2dp? not in spec though

    printf("\nFor testing dataset: %d errors, %lf probability of error.", test_PredictedErrors,probabilityErrors);

    printf("\nTP: %d, TN: %d, FP: %d, FN: %d",cM[0],cM[1],cM[2],cM[3]);

/*    printf("%s\n", train[190].outcome);
    //cm_value(train[0].outcome, predicted);
    printf("A_outcome %s\n",train[0].outcome);*/
}

void calcTrainErrors(){
    struct Dataset *test = NULL; // Initialize pointer
    int len = getTrainingData(&test); // Pass address of pointer
    printf("%d\n", len);
    if (len > 0) { // Ensure len is valid before accessing test
        for (int i = 0; i < len; i++) {
            predicted = predictOutcome(test[i]);

            if (strcmp(test[i].outcome,"positive") == 0){
                actual = 1;
            } else if (strcmp(test[i].outcome,"negative") == 0){
                actual = 0;
            } else {
                actual = -1;
            }

            if (actual != predicted){
                train_PredictedErrors += 1;
            }

            //printf("%s\n", test[i].outcome);
        }
    }

    double i = TRAINING_DATA_SIZE; //assign macro to double var as you cant cast macros
    probabilityErrors = (1 / i)*train_PredictedErrors; //round to 2dp? not in spec though

    printf("\nFor training dataset: %d errors, %lf probability of error.\n", train_PredictedErrors,probabilityErrors);

}

int main() {

    rbset();
    struct Dataset *test = NULL; // Initialize pointer
    int len = getTrainingData(&test); // Pass address of pointer
    //printf("%d\n", len);
    if (len > 0) { // Ensure len is valid before accessing test
        for (int i = 0; i < len; i++) {
            printf("%d ", i);
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    printf("%c,", test[i].grid[j][k]);
                }
            }
            printf("%s\n", test[i].outcome);
        }
    }
    initData(test, len);

    calcTrainErrors();
    calc_confusion_matrix();

    //Testing gameboard for getBestPosition
    char gameBoard[3][3] = {{'x', 'x', 'o'}, {'b', 'o', 'x'}, {'b', 'b', 'b'}};
    struct Position pos = getBestPosition(gameBoard, 'o');
    printf("Returned POS: %d %d", pos.row, pos.col);
    return 0;
}