#ifndef ML_NAIVE_BAYES_H
#define ML_NAIVE_BAYES_H

#include <macros.h>
#include <importData.h>

#define TRAINING_DATA_SIZE 0.8 * DATA_SIZE
#define TESTING_DATA_SIZE  0.2 * DATA_SIZE
#define CLASSES 2

struct Position {
    int row;
    int col;
};

void calculateProbabilities(int dataset_size);
int predictOutcome(struct Dataset board);
void initData(struct Dataset *data, int len);
struct Position getBestPosition(char grid[3][3], char player);

#endif // ML_NAIVE_BAYES_H