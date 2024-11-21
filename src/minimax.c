#include <minimax.h>

int depthCounter = 0;

int max(int a, int b)
{
#if (DISABLE_ASM)
    return (a > b) ? a : b;
#else
    int result;
#ifdef __aarch64__
    __asm__(
        "mov %w0, %w1;"       // Move 'a' to result
        "cmp %w0, %w2;"       // Compare result (a) and b
        "csel %w0, %w0, %w2, ge;" // If a >= b, keep a in result; otherwise, move b to result
        : "=&r" (result)      // Output
        : "r" (a), "r" (b)    // Inputs
        : "cc"                // Clobbered flags (condition codes)
    );
#else   //x86-64
    __asm__(
        "movl %1, %%eax;"    // Move 'a' to eax
        "movl %2, %%ebx;"    // Move 'b' to ebx
        "cmpl %%ebx, %%eax;" // Compare eax and ebx
        "jge 1f;"            // If a >= b, jump to label 1
        "movl %%ebx, %%eax;" // Otherwise, move ebx to eax
        "1:;"
        "movl %%eax, %0;" // Move result back to C variable
        : "=r"(result)    // Output
        : "r"(a), "r"(b)  // Inputs
        : "%eax", "%ebx"  // Clobbered registers
    );
#endif
    return result;
#endif
}

int min(int a, int b)
{
#if (DISABLE_ASM)
    return (a < b) ? a : b;
#else
    int result;
#ifdef __aarch64__
    __asm__(
        "mov %w0, %w1;"       // Move 'a' to result
        "cmp %w0, %w2;"       // Compare result (a) and b
        "csel %w0, %w0, %w2, le;" // If a <= b, keep a in result; otherwise, move b to result
        : "=&r" (result)      // Output
        : "r" (a), "r" (b)    // Inputs
        : "cc"                // Clobbered flags (condition codes)
    );
#else   //x86-64
    __asm__(
        "movl %1, %%eax;"    // Move 'a' to eax
        "movl %2, %%ebx;"    // Move 'b' to ebx
        "cmpl %%ebx, %%eax;" // Compare eax and ebx
        "jle 1f;"            // If a <= b, jump to label 1
        "movl %%ebx, %%eax;" // Otherwise, move ebx to eax
        "1:;"
        "movl %%eax, %0;" // Move result back to C variable
        : "=r"(result)    // Output
        : "r"(a), "r"(b)  // Inputs
        : "%eax", "%ebx"  // Clobbered registers
    );
#endif
    return result;
#endif
}

struct Position findBestMove(int board[3][3])
{
    int bestVal = -1000;
    struct Position bestMove;

    struct BoardState boardStates[MAX_BOARDS];

#if !(DISABLE_LOOKUP)
    startElapseTime();
    int boardCount = loadBoardStates(boardStates);
    stopElapseTime("Loading lookup table");
#endif

    bestMove.row = ERROR;
    bestMove.col = ERROR;

    startElapseTime();
#if !(DISABLE_LOOKUP)
    if (checkAndUpdateBestMove(board, &bestMove, boardStates, boardCount))
    {
        stopElapseTime("Find best move in lookup table");
        PRINT_DEBUG("Best move found in memory: Row = %d, Col = %d\n", bestMove.row, bestMove.col);
    }
    else
#endif
    {
        startElapseTime();
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
        stopElapseTime("Minimax depth search");
        writeBestMoveToFile(board, bestMove);
    }

    depthCounter = 0;
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
    if (isMovesLeft(board) == false)
        return 0;

#if !(MINIMAX_GODMODE)
    if (depth > 2)
        return 0;
#endif

    // If this maximizer's move
    if (isMax)
    {
        int best = -1000;

        // Traverse all cells
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                // Check if cell is empty
                if (board[i][j] == EMPTY)
                {
                    // Make the move
                    board[i][j] = BOT;

                    // Call minimax recursively and choose
                    // the maximum value
                    best = max(best, minimax(board, depth + 1, !isMax));

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
                               minimax(board, depth + 1, !isMax));

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
    for (int row = 0; row < 3; row++)
    {
        if (b[row][0] == b[row][1] &&
            b[row][1] == b[row][2])
        {
            if (b[row][0] == BOT)
                return +10;
            else if (b[row][0] == PLAYER1)
                return -10;
        }
    }

    // Checking for Columns for X or O victory.
    for (int col = 0; col < 3; col++)
    {
        if (b[0][col] == b[1][col] &&
            b[1][col] == b[2][col])
        {
            if (b[0][col] == BOT)
                return +10;

            else if (b[0][col] == PLAYER1)
                return -10;
        }
    }

    // Checking for Diagonals for X or O victory.
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2])
    {
        if (b[0][0] == BOT)
            return +10;
        else if (b[0][0] == PLAYER1)
            return -10;
    }

    if (b[0][2] == b[1][1] && b[1][1] == b[2][0])
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
#if (DISABLE_ASM)
    for (int i = 0; i<3; i++) 
        for (int j = 0; j<3; j++) 
            if (board[i][j] == EMPTY) 
                return true; 
    return false; 

#else
    int result;
#ifdef __aarch64__
__asm__(
        "mov x1, #0;"           // x1 = i = 0
        "outer_loop:;"
        "cmp x1, #3;"           // if i >= 3, go to return_false
        "bge return_false;"

        "mov x2, #0;"           // x2 = j = 0
        "inner_loop:;"
        "cmp x2, #3;"           // if j >= 3, increment i
        "bge increment_i;"

        // Calculate board[i][j]
        "mov x3, x1;"           // Copy i to x3
        "lsl x4, x1, #3;"       // x4 = i * 8
        "add x3, x4, x1, lsl #2;" // x3 = i * 8 + i * 4 = i * 12
        "add x3, %1, x3;"       // x3 = board + (i * 12), points to board[i]
        "ldr w0, [x3, x2, lsl #2];" // Load board[i][j] (each int is 4 bytes)

        "cbz w0, return_true;"   // If board[i][j] == 0, go to return_true

        "add x2, x2, #1;"       // Increment j
        "b inner_loop;"

        "increment_i:;"
        "add x1, x1, #1;"       // Increment i
        "b outer_loop;"

        "return_false:;"
        "mov %w0, #0;"          // Set result to 0 (false)
        "b end;"

        "return_true:;"
        "mov %w0, #1;"          // Set result to 1 (true)

        "end:;"
        : "=r"(result)           // Output operand
        : "r"(board)             // Input operand
        : "x0", "x1", "x2", "x3", "x4" // Clobbered registers
    );
#else   //x86-64
    __asm__(
        "xor %%rbx, %%rbx;" // rbx = i = 0
        "outer_loop:;"
        "cmp $3, %%ebx;" // if i >= 3, return false
        "jge return_false;"

        "xor %%rcx, %%rcx;" // rcx = j = 0
        "inner_loop:;"
        "cmp $3, %%ecx;" // if j >= 3, increment i
        "jge increment_i;"

        // Calculate board[i][j]
        "mov %%rbx, %%rdx;"             // Copy i to rdx
        "imul $12, %%rdx, %%rdx;"       // rdx = i * 12 (calculate row offset)
        "add %1, %%rdx;"                // rdx = board + (i * 12), points to board[i]
        "mov (%%rdx, %%rcx, 4), %%eax;" // Load board[i][j]

        "test %%eax, %%eax;" // Check if board[i][j] == 0
        "jz return_true;"    // If board[i][j] == 0, return true

        "inc %%rcx;" // Increment j
        "jmp inner_loop;"

        "increment_i:;"
        "inc %%rbx;" // Increment i
        "jmp outer_loop;"

        "return_false:;"
        "mov $0, %0;" // Set result to 0 (false)
        "jmp end;"

        "return_true:;"
        "mov $1, %0;" // Set result to 1 (true)

        "end:;"
        : "=r"(result)               // Output operand
        : "r"(board)                 // Input operand
        : "rbx", "rcx", "rdx", "rax" // Clobbered registers
    );
#endif
    return result;
#endif
}

int loadBoardStates(struct BoardState boardStates[])
{
    FILE *file = fopen(FILE_BESTMOV, "r");
    if (file == NULL)
    {
        PRINT_DEBUG("%s <- File does not exist. Creating new file.\n", FILE_BESTMOV);
        FILE *file = fopen(FILE_BESTMOV, "w");
        PRINT_DEBUG("Text file created.\n");
        fclose(file);
        return 0; // No boards loaded
    }
    PRINT_DEBUG("File exist. Checking.\n");
    int count = 0;
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_BOARDS)
    {
        // Parse the line
        char *token = strtok(line, ",");
        int index = 0;

        // Read the board condition
        while (token != NULL && index < 9)
        {
            if (strcmp(token, "x") == 0)
            {
                boardStates[count].board[index / 3][index % 3] = BOT;
            }
            else if (strcmp(token, "o") == 0)
            {
                boardStates[count].board[index / 3][index % 3] = PLAYER1;
            }
            else
            {
                boardStates[count].board[index / 3][index % 3] = EMPTY;
            }
            token = strtok(NULL, ",");
            index++;
        }

        // Read the best move
        if (token != NULL)
        {
            boardStates[count].bestMove.row = atoi(token);
            token = strtok(NULL, ",");
            boardStates[count].bestMove.col = atoi(token);
        }
        count++;
    }

    fclose(file);
    return count; // Return the number of boards loaded
}

bool checkAndUpdateBestMove(int board[3][3], struct Position *bestMove, struct BoardState boardStates[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (memcmp(board, boardStates[i].board, sizeof(boardStates[i].board)) == 0)
        {
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

void writeBestMoveToFile(int board[3][3], struct Position bestMove)
{
    FILE *file = fopen(FILE_BESTMOV, "a"); // Open the file for appending
    if (file == NULL)
    {
        PRINT_DEBUG("Error opening file for writing. -> %s\n", FILE_BESTMOV);
        return;
    }

    // Write the board state to the file
    for (int j = 0; j < 3; j++)
    {
        for (int k = 0; k < 3; k++)
        {
            if (board[j][k] == PLAYER1)
            {
                fprintf(file, "o,");
                PRINT_DEBUG("o,");
            }
            else if (board[j][k] == BOT)
            {
                fprintf(file, "x,");
                PRINT_DEBUG("x,");
            }
            else
            {
                fprintf(file, "b,");
                PRINT_DEBUG("b,");
            }
        }
    }
    // Write the best move to the file
    fprintf(file, "%d,%d\n", bestMove.row, bestMove.col);
    if (fprintf(file, "%d,%d\n", bestMove.row, bestMove.col) < 0)
    {
        PRINT_DEBUG("Error writing best move to file. -> %s\n", FILE_BESTMOV);
    }
    PRINT_DEBUG("\nAttempting to write best move to file: Row = %d, Col = %d\n", bestMove.row, bestMove.col);
    PRINT_DEBUG("New best move written to file.\n");
    fclose(file);
}