#include <importData.h>

void readDataset(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
    }

    char line[100];
    
    // get an array of random int where each position is different
    getRandomNo(randomNo);

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
        }
    }
    fclose(file);
    splitFile();
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

    for (int i = eighty; DATA_SIZE > i; i++) 
    {
        for (int row = 0; 3 > row; row++) 
        {
            for (int col = 0; 3 > col; col++) 
            {
                fprintf(testFile, "%c,", data[i].grid[row][col]);
            }
        }
        fprintf(testFile, "%s\n", data[i].outcome);
    }
    fclose(testFile);
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

// Assign an index for each move "x", "o" or "b"
int assignMoveIndex(char move) {
    switch(move) {
        case 'x': return 0;
        case 'o': return 1;
        case 'b': return 2;
        default: return -1;
    }
}

// int main()
// {
//     readDataset(RES_PATH""DATA_PATH);
//     return 0;
// }