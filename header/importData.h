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

struct Dataset data[DATA_SIZE];

// to write to directory before 
const char *trainingFile = RES_PATH""TRAIN_PATH""DATA_PATH;
const char *testingFile =  RES_PATH""TEST_PATH""DATA_PATH;

int readDataset(const char* filename, bool split);
int splitFile();
void getRandomNo(int random[DATA_SIZE]);
int assignMoveIndex(char move);