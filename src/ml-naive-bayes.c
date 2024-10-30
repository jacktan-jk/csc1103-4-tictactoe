#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// changed to 100 for testing. make sure to chg back
#define DATA_SIZE 100
#define CLASSES 2


struct Dataset {
    char grid[3][3];
    char outcome[9];
};

struct Dataset data[DATA_SIZE];

// to write to directory before 
const char *trainingFile = "../training-tic-tac-toe.data";
const char *testingFile = "../testing-tic-tac-toe.data";

int randomNo[DATA_SIZE];

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

void getRandomNo(int random[DATA_SIZE]) {
    int count = 0;
    srand(time(NULL));

    // initialize all to 0 for proper check
    int check[DATA_SIZE];
    for (int i = 0; DATA_SIZE > i; i++) {
        check[i] = 0;
    } 

    while (DATA_SIZE > count) {
        int randNo = rand() % DATA_SIZE;
        if (check[randNo] == 0) {
            check[randNo] = 1;
            random[count] = randNo;
            count++;
        }
    }
}  


// split into 80 - 20; write into training and testing file accordingly
void splitFile() {
    // get 80% and 20% respectively
    int eighty = 0.8 * DATA_SIZE;
    int twenty = 0.2 * DATA_SIZE;

    // write into training dataset
    FILE *trainFile;
    trainFile = fopen(trainingFile, "w");
    if (!trainFile) {
        printf("Error opening file. \n");
    }

    for (int i = 0; eighty > i; i++) {
        for (int row = 0; 3 > row; row++) {
            for (int col = 0; 3 > col; col++) {
                fprintf(trainFile, "%c,", data[i].grid[row][col]);
            }
        }
        fprintf(trainFile, "%s\n", data[i].outcome);
    }

    fclose(trainFile);

    // write into testing dataset
    FILE *testFile;
    testFile = fopen(testingFile, "w");
    if (!testFile) {
        printf("Error opening file. \n");
    }

    for (int i = eighty; DATA_SIZE > i; i++) {
        for (int row = 0; 3 > row; row++) {
            for (int col = 0; 3 > col; col++) {
                fprintf(testFile, "%c,", data[i].grid[row][col]);
            }
        }
        fprintf(testFile, "%s\n", data[i].outcome);
    }

    fclose(testFile);

}

void readDataset(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
    }

    char line[100];
    
    // get an array of random int where each position is different
    getRandomNo(randomNo);

    // split 80/20 into training / testing - write into respective files 
    int eighty = 0.8 * DATA_SIZE;
    int twenty = 0.2 * DATA_SIZE;

    for (int i = 0; i < DATA_SIZE && fgets(line, sizeof(line), file); i++) {
        // Get first token with delimiter being ","
        char *token = strtok(line, ",");
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (token != NULL) {
                    data[randomNo[i]].grid[row][col] = token[0];
                    token = strtok(NULL, ",");
                }
            }
        }

        if (token != NULL) {
            strncpy(data[randomNo[i]].outcome, token, sizeof(data[randomNo[i]].outcome) - 1); 
            // Get outcome class count
            if (strcmp(data[randomNo[i]].outcome, "positive") == 0) {
                positive_count++;
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        int moveIndex = assignMoveIndex(data[randomNo[i]].grid[row][col]);
                        if (moveIndex != -1) {
                            positiveMoveCount[row][col][moveIndex]++;
                        }
                    }
                }
            }
            else if (strcmp(data[randomNo[i]].outcome, "negative") == 0) {
                negative_count++;
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        int moveIndex = assignMoveIndex(data[randomNo[i]].grid[row][col]);
                        if (moveIndex != -1) {
                            negativeMoveCount[row][col][moveIndex]++;
                        }
                    }
                }
            }
        }



        // Output board and outcome
        // printf("\n------------------");
        // printf("\nBoard for Game %d: \n", i + 1);
        // for (int row = 0; row < 3; row++) {
        //     for (int col = 0; col < 3; col++) {
        //         printf("%c ", data[i].grid[row][col]);
        //     }
        //     printf("\n");
        // }
        // printf("\nOutcome: %s\n", data[i].outcome);
        // printf("------------------\n");
    }

    fclose(file);

    splitFile();


}



void calculateProbabilities(int dataset_size) {
    // Calculate class probability
    positiveClassProbability =  (double) positive_count/dataset_size;
    negativeClassProbability = (double) negative_count/dataset_size;
    printf("Positive Class Probability: %lf\n", positiveClassProbability);
    printf("Negative Class Probability: %lf\n", negativeClassProbability);

    // Calculate conditional probability
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            for (int moveIndex = 0; moveIndex < 3; moveIndex++) {
                char move;
                if (moveIndex == 0) {
                    move = 'x';
                } else if (moveIndex == 1) {
                    move = 'o';
                } else {
                    move = 'b';
                }
                double positiveProbability = (double) positiveMoveCount[row][col][moveIndex]/positive_count;
                double negativeProbability = (double) negativeMoveCount[row][col][moveIndex]/negative_count;
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


int main() {
    readDataset("../tic-tac-toe.data");

    //     for (int a = 0; DATA_SIZE > a; a++) {
    //     for (int row = 0; 3 > row; row++) {
    //         for (int col = 0; 3 > col; col++) {
    //             // printf("%c ", data[a].grid[row][col]);
    //         }
    //         printf("\n");
    //     }

    // }

    // for (int i = 0; DATA_SIZE > i; i++) {
    //     printf("%d: %s\n", i, data[i]);
    // }

    // printf("test: %c\n", data[0].grid[0][1]);

    // for (int a = 0; DATA_SIZE > a; a++) {
    //     printf("%d,", randomNo[a]);
    // }


    

    // calculateProbabilities(DATA_SIZE);
    // char testBoard[3][3] = {{'x', 'x', 'x'}, {'x', 'o', 'o'}, {'x', 'o', 'o'}};
    // printf("\n------------------");
    // printf("\nBoard for Test Game:\n");
    // for (int row = 0; row < 3; row++) {
    //     for (int col = 0; col < 3; col++) {
    //         printf("%c ", testBoard[row][col]);
    //     }
    //     printf("\n");
    // }
    // printf("------------------\n");

    // predictOutcome(testBoard);

    return 0;
}
