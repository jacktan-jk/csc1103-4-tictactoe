#include <macros.h>
#include <importData.h>

#define TRAINING_DATA_SIZE 0.8 * DATA_SIZE
#define TESTING_DATA_SIZE  0.2 * DATA_SIZE
#define CLASSES 2

struct Position {
    int row;
    int col;
};

int assignMoveIndex(char move);
void calculateProbabilities(int dataset_size);
void predictOutcome(char grid[3][3]);
struct Position getBestPosition(char grid[3][3], char player);