#ifndef IMPORTDATA_H
#define IMPORTDATA_H

#include <macros.h>

// changed to 100 for testing. make sure to chg back
#define RES_PATH "resources/"
#define DATA_PATH "tic-tac-toe.data"
#define TRAIN_PATH "training-"
#define TEST_PATH "testing-"

struct Dataset {
    char grid[3][3];
    char outcome[9];
};

int readDataset(const char* filename, bool split);
int splitFile();
void getRandomNo(int random[DATA_SIZE]);
int getTrainingData(struct Dataset **d);
int getTestingData(struct Dataset **d);
int assignMoveIndex(char move);

#endif // IMPORTDATA_H