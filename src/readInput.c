#include <readInput.h>
// data[x, m] where x is the dataset row and n is the square box no. 

// output[data_rows] wheere data_rows is the classification output and the key (same key as data[][] for reference to the 2d array)
char value[256];
int i = 0;

// read the data from the .data file.
struct allData getData() {

    struct allData dataList;

    char path[] = "resources/tic-tac-toe.data";
    FILE *fptr = fopen(path, "r");

    if (access(path, F_OK) != SUCCESS || fptr == NULL) 
    {
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
                a++;
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
            else 
            {
                strcpy(dataList.testingData[x - TRAINING_SET][y], dataList.data[x][y]);
            }
        }
    }

    return dataList;

}
