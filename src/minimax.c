#include <minimax.h>

int depthCounter = 0;

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Helper function to get the minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

struct Position findBestMove(int board[3][3]) 
{ 
    int bestVal = -1000; 
    struct Position bestMove; 

    struct BoardState boardStates[MAX_BOARDS];
    int boardCount = loadBoardStates(boardStates);

    bestMove.row = ERROR; 
    bestMove.col = ERROR; 

    if (checkAndUpdateBestMove(board, &bestMove, boardStates, boardCount)) {
        PRINT_DEBUG("Best move found in memory: Row = %d, Col = %d\n", bestMove.row, bestMove.col);
        
    } else {
        PRINT_DEBUG("Best move not found in memory\n");
        // Traverse all cells, evaluate minimax function for 
        // all empty cells. And return the cell with optimal 
        // value.
        for (int i = 0; i < 3; i++) 
        { 
            for (int j = 0; j < 3; j++) 
            { 
                // Check if cell is empty 
                if (board[i][j] == EMPTY) 
                { 
                    // Make the move 
                    board[i][j] = BOT; 

                    // compute evaluation function for this 
                    // move. 
                    int moveVal = minimax(board, 0, false);
                    PRINT_DEBUG("[DEBUG] Depth exited at -> %d\n", depthCounter);
                    // Undo the move 
                    board[i][j] = EMPTY; 

                    // If the value of the current move is more than the best value, then update best move 
                    if (moveVal > bestVal) 
                    { 
                        bestMove.row = i; 
                        bestMove.col = j; 
                        bestVal = moveVal; 
                    } 
                } 
            } 
        }
        writeBestMoveToFile(board, bestMove);
    }

    //printFileContents();
    depthCounter=0;
    return bestMove; 
}

int minimax(int board[3][3], int depth, bool isMax) 
{ 
#if DEBUG
    depthCounter++;
#endif
    int score = evaluate(board); 
    // If Maximizer has won the game return his/her 
    // evaluated score 
    if (score == 10) 
        return score; 
  
    // If Minimizer has won the game return his/her 
    // evaluated score 
    if (score == -10) 
        return score; 
  
    // If there are no more moves and no winner then 
    // it is a tie 
    if (isMovesLeft(board)==false) 
        return 0; 
    
    if(depth>2)
        return 0;

    // If this maximizer's move 
    if (isMax) 
    { 
        int best = -1000; 
  
        // Traverse all cells 
        for (int i = 0; i<3; i++) 
        { 
            for (int j = 0; j<3; j++) 
            { 
                // Check if cell is empty 
                if (board[i][j] == EMPTY) 
                { 
                    // Make the move 
                    board[i][j] = BOT; 
  
                    // Call minimax recursively and choose 
                    // the maximum value 
                    best = max( best, minimax(board, depth+1, !isMax) ); 

                    // Undo the move 
                    board[i][j] = EMPTY; 
                } 
            } 
        } 
        return best; 
    } 
  
    // If this minimizer's move 
    else
    { 
        int best = 1000; 
  
        // Traverse all cells 
        for (int i = 0; i < 3; i++) 
        { 
            for (int j = 0; j < 3; j++) 
            { 
                // Check if cell is empty 
                if (board[i][j] == EMPTY) 
                { 
                    // Make the move 
                    board[i][j] = PLAYER1; 
  
                    // Call minimax recursively and choose 
                    // the minimum value 
                    best = min(best, 
                           minimax(board, depth+1, !isMax)); 
  
                    // Undo the move 
                    board[i][j] = EMPTY; 
                } 
            } 
        } 
        return best; 
    } 
} 

int evaluate(int b[3][3]) 
{ 
    // Checking for Rows for X or O victory. 
    for (int row = 0; row<3; row++) 
    { 
        if (b[row][0]==b[row][1] && 
            b[row][1]==b[row][2]) 
        { 
            if (b[row][0] == BOT) 
                return +10; 
            else if (b[row][0] == PLAYER1) 
                return -10; 
        } 
    } 
  
    // Checking for Columns for X or O victory. 
    for (int col = 0; col<3; col++) 
    { 
        if (b[0][col]==b[1][col] && 
            b[1][col]==b[2][col]) 
        { 
            if (b[0][col] == BOT) 
                return +10; 
  
            else if (b[0][col] == PLAYER1) 
                return -10; 
        } 
    } 
  
    // Checking for Diagonals for X or O victory. 
    if (b[0][0]==b[1][1] && b[1][1]==b[2][2]) 
    { 
        if (b[0][0] == BOT) 
            return +10; 
        else if (b[0][0] == PLAYER1) 
            return -10; 
    } 
  
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0]) 
    { 
        if (b[0][2] == BOT) 
            return +10; 
        else if (b[0][2] == PLAYER1) 
            return -10; 
    } 
  
    // Else if none of them have won then return 0 
    return 0; 
} 

bool isMovesLeft(int board[3][3]) 
{ 
    for (int i = 0; i<3; i++) 
        for (int j = 0; j<3; j++) 
            if (board[i][j] == EMPTY) 
                return true; 
    return false; 
} 

int loadBoardStates(struct BoardState boardStates[]) {
    FILE *file = fopen(FILE_BESTMOV, "r");
    if (file == NULL) {
        PRINT_DEBUG("%s <- File does not exist. Creating new file.\n", FILE_BESTMOV);
        FILE *file = fopen(FILE_BESTMOV, "w");
        PRINT_DEBUG("Text file created.\n");
        fclose(file);
        return 0; // No boards loaded
    }
    PRINT_DEBUG("File exist. Checking.\n");
    int count = 0;
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_BOARDS) {
        // Parse the line
        char *token = strtok(line, ",");
        int index = 0;

        // Read the board condition
        while (token != NULL && index < 9) {
            if (strcmp(token, "x") == 0) {
                boardStates[count].board[index / 3][index % 3] = BOT;
            } else if (strcmp(token, "o") == 0) {
                boardStates[count].board[index / 3][index % 3] = PLAYER1;
            } else {
                boardStates[count].board[index / 3][index % 3] = EMPTY;
            }
            token = strtok(NULL, ",");
            index++;
        }

        // Read the best move
        if (token != NULL) {
            boardStates[count].bestMove.row = atoi(token);
            token = strtok(NULL, ",");
            boardStates[count].bestMove.col = atoi(token);
        }
        count++;
    }

    fclose(file);
    return count; // Return the number of boards loaded
}

bool checkAndUpdateBestMove(int board[3][3], struct Position *bestMove, struct BoardState boardStates[], int count) {
    for (int i = 0; i < count; i++) {
        if (memcmp(board, boardStates[i].board, sizeof(boardStates[i].board)) == 0) {
            // Board matches, update the best move
            *bestMove = boardStates[i].bestMove;
            PRINT_DEBUG("Found position in lookup table\n");
            PRINT_DEBUG("Best Move = R:%d C:%d\n", bestMove->row, bestMove->col);
            return bestMove; // Board matches, return the best move
            
        }
    }
    PRINT_DEBUG("Position not found in lookup table\n");
    return false; // No matching board found
}
void writeBestMoveToFile(int board[3][3], struct Position bestMove) {
    FILE *file = fopen(FILE_BESTMOV, "a"); // Open the file for appending
    if (file == NULL) {
        PRINT_DEBUG("Error opening file for writing. -> %s\n", FILE_BESTMOV);
        return;
    }

    // Write the board state to the file
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
            if (board[j][k] == PLAYER1) {
                fprintf(file, "o,");
                PRINT_DEBUG("o,");
            } else if (board[j][k] == BOT) {
                fprintf(file, "x,");
                PRINT_DEBUG("x,");
            } else {
                fprintf(file, "b,");
                PRINT_DEBUG("b,");
            }
        }
    }
    // Write the best move to the file
    fprintf(file, "%d,%d\n", bestMove.row, bestMove.col);
    if (fprintf(file, "%d,%d\n", bestMove.row, bestMove.col) < 0) {
        PRINT_DEBUG("Error writing best move to file. -> %s\n", FILE_BESTMOV);
    }
    PRINT_DEBUG("\nAttempting to write best move to file: Row = %d, Col = %d\n", bestMove.row, bestMove.col);
    PRINT_DEBUG("New best move written to file.\n\n");
    fclose(file);
}

void printFileContents() {
    if(DEBUG)
    {
        FILE *file = fopen(FILE_BESTMOV, "r");
        if (file == NULL) {
            PRINT_DEBUG("File does not exist or cannot be opened. -> %s\n", FILE_BESTMOV);
            return;
        }

        PRINT_DEBUG("Contents of %s:\n", FILE_BESTMOV);
        char line[100];
        while (fgets(line, sizeof(line), file) != NULL) {
            PRINT_DEBUG("%s", line);
        }
        fclose(file);
    }
}