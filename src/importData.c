#include <importData.h>

int len_train = 0;
int len_test = 0;
int randomNo[DATA_SIZE];
struct Dataset data[DATA_SIZE];

// to write to directory before 
const char *trainingFile = RES_PATH""TRAIN_PATH""DATA_PATH;
const char *testingFile =  RES_PATH""TEST_PATH""DATA_PATH;

int readDataset(const char* filename, bool split) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        PRINT_DEBUG("[ERROR] Error opening file.\n");
        return ERROR;
    }
    
    if(split)
    {
        // get an array of random int where each position is different
        getRandomNo(randomNo);
    }

    char line[100];
    for (int i = 0; i < DATA_SIZE && fgets(line, sizeof(line), file); i++) {
        // Get first token with delimiter being ","
        char *token = strtok(line, ",");
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (token != NULL) {
                    data[split ? randomNo[i] : i].grid[row][col] = token[0];
                    token = strtok(NULL, ",");
                }
            }
        }

        if (token != NULL) {
            strncpy(data[split ? randomNo[i] : i].outcome, token, sizeof(data[split ? randomNo[i] : i].outcome) - 1);
        }
    }
    fclose(file);

    if(split)
    {
        return splitFile();
    }
    return SUCCESS;
}

// split into 80 - 20; write into training and testing file accordingly
int splitFile() {
    // get 80% and 20% respectively
    int eighty = len_train = 0.8 * DATA_SIZE;
    len_test = 0.2 * DATA_SIZE;

    // write into training dataset
    FILE *trainFile;
    trainFile = fopen(trainingFile, "w");
    if (!trainFile) {
        PRINT_DEBUG("[ERROR] Error opening file.\n");
        return BAD_PARAM;
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
        PRINT_DEBUG("[ERROR] Error opening file.\n");
        return BAD_PARAM;
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
    return SUCCESS;
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

int getTrainingData(struct Dataset **d) 
{
    memset(data, 0, len_train * sizeof(struct Dataset));
    readDataset(trainingFile, false);
    *d = data;
    return len_train;
}

int getTestingData(struct Dataset **d) 
{
    memset(data, 0, len_test * sizeof(struct Dataset));
    readDataset(testingFile, false);
    *d = data;
    return len_test;
}
