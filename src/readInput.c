#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DATA_ROWS 15
#define BOX 9
#define TRAINING_SET (int)(0.8 * DATA_ROWS)
#define TESTING_SET (DATA_ROWS - TRAINING_SET)
#define DATA_NO 10 // total no. of variables in data set


// data[x, m] where x is the dataset row and n is the square box no. 

// output[data_rows] wheere data_rows is the classification output and the key (same key as data[][] for reference to the 2d array)
char value[256];
int i = 0;

struct allData {
    char data[DATA_ROWS][BOX][20];
    char dataOutput[DATA_ROWS][20];
    char trainingData[TRAINING_SET][BOX][20];
    char trainingOutput[TRAINING_SET][10];
    char testingData[TESTING_SET][BOX][20];
    char testingOutput[TESTING_SET][20];
};

// read the data from the .data file.
struct allData getData() {
    struct allData dataList;
    FILE *fptr = fopen("tic-tac-toe.data", "r");
    if (fptr == NULL) {
        perror("Error opening file.");
        return dataList;
    }

    printf("Training_SET: %d", TRAINING_SET);

    while (fgets(value, sizeof(value), fptr) && DATA_ROWS > i) {
            // split by ","
            char* token = strtok(value, ",");
            int a = 0;  
            while (token != NULL && DATA_NO > a) {
                // printf("%c, %d\n", token, a);
                strcpy(dataList.data[i][a], token);
                // last position. store the classification output to output[] array
                if (a == DATA_NO - 1) {
                    strcpy(dataList.dataOutput[i], token);
                }
                a ++;
                token = strtok(NULL, ",");
            }
            i++; 
        }
    
    fclose(fptr);

    // split the data accordingly. first 80% is training data. 
    // last 20% is testing data.
    for (int x = 0; DATA_ROWS > x; x++) {
        if (TRAINING_SET > x) {
            strcpy(dataList.trainingOutput[x], dataList.dataOutput[x]);
        } else {
            strcpy(dataList.testingOutput[x - TRAINING_SET], dataList.dataOutput[x]);
        }

        for (int y = 0; BOX > y; y++) {
            if (TRAINING_SET > x) {
                strcpy(dataList.trainingData[x][y], dataList.data[x][y]);
            }
         else {
            strcpy(dataList.testingData[x - TRAINING_SET][y], dataList.data[x][y]);
        }
        }
    }

    return dataList;

}

// split the data - 80% training data 20% testing 
// returns the training data and testing data in the struct allData
// struct allData splitData(struct allData mainData) {
//         int reset = 0;
//         for (int a = 0; DATA_ROWS > a; a++) {
//             if (TRAINING_SET > a) {
//                 for (int b = 0; BOX > b; b++) {
//                     // printf("data[%d][%d]: %s\n", a, b, mainData.data[a][b]);
//                     strcpy(mainData.trainingData[a], mainData.data[a]);
//             }
//             strcpy(mainData.trainingOutput[a], mainData.dataOutput[a]); 
        
//         } else {
//             for (int b = 0; BOX > b; b++) {
//                 strcpy(mainData.testingData[reset][b], mainData.data[a][b]);
//                 reset++;
//             }
//             strcpy(mainData.testingOutput[reset], mainData.dataOutput[a]);
//             }       
//         }
//         return mainData; 
//     }


// test it out
int main() {
    
    struct allData test = getData();

    // for (int a = 0; 2 > a; a++) {
    //     for (int b = 0; BOX > b; b++) {
    //         printf("data[%d][%d]: %s\n", a, b, test.data[a][b]);
    //     }
    //     printf("%s\n", test.dataOutput[a]);
    // }

    for (int a = 0; TRAINING_SET > a; a++) {
        for (int b = 0; BOX > b; b++) {
            printf("training data[%d][%d]: %s\n", a, b, test.trainingData[a][b]);
        }
        printf("%s\n", test.trainingOutput[a]);
    }

    for (int a = 0; TESTING_SET > a; a++) {
        for (int b = 0; BOX > b; b++) {
            printf("testing data[%d][%d]: %s\n", a, b, test.testingData[a][b]);
        }
        printf("%s\n", test.trainingOutput[a]);
    } 

    return 0;
}