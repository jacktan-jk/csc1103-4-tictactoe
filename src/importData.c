#include <importData.h>

int len_train = 0;
int len_test = 0;
int randomNo[DATA_SIZE];

int readDataset(const char* filename, bool split) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
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
    int twenty = len_test = 0.2 * DATA_SIZE;

    // write into training dataset
    FILE *trainFile;
    trainFile = fopen(trainingFile, "w");
    if (!trainFile) {
        printf("Error opening file. \n");
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
        printf("Error opening file. \n");
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

int getTrainingData(struct Dataset *d)
{
    if(data == NULL)
    {
        return BAD_PARAM;
    }
    memset(data, 0, len_train);
    readDataset(trainingFile, false);
    d = data;
    return len_train;
}

int getTestingData(struct Dataset *d)
{
    if(data == NULL)
    {
        return BAD_PARAM;
    }
    memset(data, 0, len_test);
    readDataset(testingFile, false);
    d = data;
    return len_test;
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
//     readDataset(RES_PATH""DATA_PATH, true);
//     struct Dataset *slut;
//     int len = getTrainingData(slut);
//     printf("%d\n", len);
//     for(int i = 0; i < len; i++)
//     {
//         printf("%d ", i);
//         for(int j = 0; j < 3; j++)
//         {
//             for(int k = 0; k < 3; k++)
//             {
//                 printf("%c,", data[i].grid[j][k]);
//             }
//         }
//         printf("%s\n", data[i].outcome);
//     }
//     return 0;
// }