#include <importData.h>

/**  
 * @var int len_train
 * @brief Global variable to store the number of training dataset entries.
 * 
 * This variable tracks the size of the training dataset after splitting.
 *  
 * @var int len_test
 * @brief Global variable to store the number of testing dataset entries.
 * 
 * This variable tracks the size of the testing dataset after splitting. 
 *  
 * @var int randomNo[DATA_SIZE]
 * @brief Global array to store unique random indices for dataset splitting.
 * 
 * This array stores randomized indices used to split the dataset into training and testing subsets.
 * 
 * @var struct Dataset data[DATA_SIZE]
 * @brief Global array to store the dataset.
 * 
 * This array holds the Tic-Tac-Toe board states and their corresponding outcomes.
 *
 * @var const char *trainingFile
 * @brief Global variable to store the path for the training dataset file.
 * 
 * This variable holds the full path to the training dataset file for reading and writing.
 * 
 * @var const char *testingFile
 * @brief Global variable to store the path for the testing dataset file.
 * 
 * This variable holds the full path to the testing dataset file for reading and writing.
 * 
 */
int len_train = 0;
int len_test = 0;
int randomNo[DATA_SIZE];
struct Dataset data[DATA_SIZE];

// to write to directory before
const char *trainingFile = RES_PATH "" TRAIN_PATH "" DATA_PATH;
const char *testingFile = RES_PATH "" TEST_PATH "" DATA_PATH;

/**  
 * @brief Reads a dataset from a file and optionally randomizes entries for training and testing.
 * 
 * Opens a file to read each line as a Tic Tac Toe board state, populating a grid structure 
 * where 'x', 'o', and 'b' represent the Bot, Player 1, and empty cells, respectively. 
 * Each board state is followed by an outcome that is stored within the dataset. 
 * If `split` is true, entries are randomized using an array of unique indices for shuffling.
 * 
 * @param filename The name of the dataset file to read.
 * @param split Boolean indicating whether to randomize entries for dataset splitting.
 * @return int SUCCESS (0) if reading is successful, ERROR (-1) if the file cannot be opened, 
 *         or the return value of `splitFile()` if `split` is enabled.
 * 
 * @see getRandomNo, splitFile
 */
int readDataset(const char *filename, bool split)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        PRINT_DEBUG("[ERROR] Error opening file.\n");
        return ERROR;
    }

    if (split)
    {
        // get an array of random int where each position is different
        getRandomNo(randomNo);
    }

    char line[100];
    for (int i = 0; i < DATA_SIZE && fgets(line, sizeof(line), file); i++)
    {
        // Get first token with delimiter being ","
        char *token = strtok(line, ",");
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                if (token != NULL)
                {
                    data[split ? randomNo[i] : i].grid[row][col] = token[0];
                    token = strtok(NULL, ",");
                }
            }
        }

        if (token != NULL)
        {
            strncpy(data[split ? randomNo[i] : i].outcome, token, sizeof(data[split ? randomNo[i] : i].outcome) - 1);
        }
    }
    fclose(file);

    if (split)
    {
        return splitFile();
    }
    return SUCCESS;
}

/** 
 * @brief Splits the dataset into training and testing files with an 80-20 ratio.
 * 
 * This function separates the dataset into two parts: 80% for training and 20% for testing. 
 * The training portion is written to `trainingFile`, and the testing portion is written to 
 * `testingFile`. Each entry consists of a 3x3 grid representing the Tic Tac Toe board and 
 * the outcome of that board.
 * 
 * @return int SUCCESS (0) if both files are written successfully, BAD_PARAM (-1) if either 
 *         file cannot be opened.
 * 
 * @see data, trainingFile, testingFile
 */
int splitFile()
{
    // get 80% and 20% respectively
    int eighty = len_train = 0.8 * DATA_SIZE;
    len_test = 0.2 * DATA_SIZE;

    // write into training dataset
    FILE *trainFile;
    trainFile = fopen(trainingFile, "w");
    if (!trainFile)
    {
        PRINT_DEBUG("[ERROR] Error opening file.\n");
        return BAD_PARAM;
    }

    for (int i = 0; eighty > i; i++)
    {
        for (int row = 0; 3 > row; row++)
        {
            for (int col = 0; 3 > col; col++)
            {
                fprintf(trainFile, "%c,", data[i].grid[row][col]);
            }
        }
        fprintf(trainFile, "%s\n", data[i].outcome);
    }

    fclose(trainFile);

    // write into testing dataset
    FILE *testFile;
    testFile = fopen(testingFile, "w");
    if (!testFile)
    {
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

/** 
 * @brief Generates an array of unique random integers within the range of the dataset size.
 * 
 * This function populates an array with unique random integers between 0 and `DATA_SIZE - 1`. 
 * It ensures that each integer appears only once by checking a `check` array to track used 
 * indices. This can be used for randomizing the order of data for splitting purposes.
 * 
 * @param random Array to store the generated unique random integers.
 * 
 * @see DATA_SIZE
 */
void getRandomNo(int random[DATA_SIZE])
{
    int count = 0;
    srand(time(NULL));

    // initialize all to 0 for proper check
    int check[DATA_SIZE];
    for (int i = 0; DATA_SIZE > i; i++)
    {
        check[i] = 0;
    }

    while (DATA_SIZE > count)
    {
        int randNo = rand() % DATA_SIZE;
        if (check[randNo] == 0)
        {
            check[randNo] = 1;
            random[count] = randNo;
            count++;
        }
    }
}

/** 
 * @brief Retrieves the training data from a file and returns its length.
 * 
 * This function initializes the `data` array to zero for the length of the training set, reads 
 * the dataset from the specified `trainingFile`, and assigns the `data` pointer to the provided 
 * dataset pointer. Returns the length of the training data loaded.
 * 
 * @param[out] d Pointer to a dataset pointer that will reference the loaded training data array.
 * 
 * @return The number of training entries loaded (i.e., `len_train`).
 * 
 * @see readDataset, trainingFile
 */
int getTrainingData(struct Dataset **d)
{
    memset(data, 0, len_train * sizeof(struct Dataset));
    readDataset(trainingFile, false);
    *d = data;
    return len_train;
}

/** 
 * @brief Retrieves the testing data from a file and returns its length.
 * 
 * This function zeroes out the `data` array for the length of the testing set, reads the 
 * dataset from the specified `testingFile`, and assigns the `data` pointer to the provided 
 * dataset pointer. Returns the length of the testing data loaded.
 * 
 * @param[out] d Pointer to a dataset pointer that will reference the loaded testing data array.
 * 
 * @return The number of testing entries loaded (i.e., `len_test`).
 * 
 * @see readDataset, testingFile
 */
int getTestingData(struct Dataset **d)
{
    memset(data, 0, len_test * sizeof(struct Dataset));
    readDataset(testingFile, false);
    *d = data;
    return len_test;
}
