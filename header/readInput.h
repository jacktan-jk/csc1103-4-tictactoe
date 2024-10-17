#include <macros.h>
#include <strings.h>
#include <unistd.h>

#define DATA_ROWS 958
#define BOX 9
#define TRAINING_SET (int)(0.8 * DATA_ROWS)
#define TESTING_SET (DATA_ROWS - TRAINING_SET)
#define DATA_NO 10 // total no. of variables in data set

struct allData {
    char data[DATA_ROWS][BOX][20];
    char dataOutput[DATA_ROWS][20];
    char trainingData[TRAINING_SET][BOX][20];
    char trainingOutput[TRAINING_SET][10];
    char testingData[TESTING_SET][BOX][20];
    char testingOutput[TESTING_SET][20];
};

struct allData getData();