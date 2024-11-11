#ifndef ML_NAIVE_BAYES_H
#define ML_NAIVE_BAYES_H

#include <macros.h>
#include <importData.h>

#define TRAINING_DATA_SIZE 0.8 * DATA_SIZE
#define TESTING_DATA_SIZE 0.2 * DATA_SIZE
#define CLASSES 2

int assignMoveIndex(char move);
void calculateProbabilities(int dataset_size);
void resetTrainingData();
int initData();
int predictOutcome(struct Dataset board);
void calcTrainErrors();
void calcConfusionMatrix();
struct Position getBestPosition(int grid[3][3], char player);

int getTruthValue(char *str1);
void assignCMValue(int actual, int predicted);
void debugDataset(struct Dataset *data, int len);

#endif // ML_NAIVE_BAYES_H
