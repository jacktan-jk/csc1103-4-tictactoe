#include <minimax.h>

int depthCounter = 0;

/**  
 * @brief Returns the maximum of two integers using inline assembly.
 * 
 * This function compares two integers, `a` and `b`, and returns the greater of the two. 
 * It uses inline assembly to perform the comparison and conditional move, making the operation more efficient.
 * 
 * @param a The first integer.
 * @param b The second integer.
 * 
 * @return The maximum of `a` and `b`.
 */
int max(int a, int b)
{
    int result;
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
    return result;
}

/**  
 * @brief Returns the minimum of two integers using inline assembly.
 * 
 * This function compares two integers, `a` and `b`, and returns the smaller of the two. 
 * It uses inline assembly to perform the comparison and conditional move, optimizing the operation.
 * 
 * @param a The first integer.
 * @param b The second integer.
 * 
 * @return The minimum of `a` and `b`.
 */
int min(int a, int b)
{
    int result;
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
    return result;
}

/**  
 * @brief Finds the best move for the bot in the Tic-Tac-Toe game.
 * 
 * This function first checks if the best move is already stored in memory by 
 * looking through previous board states. If the move is found, it is returned. 
 * If no best move is found in memory, it traverses all the empty cells on the 
 * board, evaluates the potential moves using the minimax algorithm, and returns 
 * the optimal move.
 * 
 * @param board A 3x3 array representing the current Tic-Tac-Toe board.
 * 
 * @return The best move for the bot as a struct Position containing the row and column.
 * 
 * @see minimax, loadBoardStates, checkAndUpdateBestMove, writeBestMoveToFile
 */
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

/**  
 * @brief Implements the Minimax algorithm to evaluate the best move for the bot.
 * 
 * The function recursively evaluates all possible moves using the Minimax 
 * algorithm. It returns the best score for the current player (maximizer or 
 * minimizer) based on the game state. The algorithm chooses the optimal move 
 * for the bot and evaluates the game state at each depth. The depth is capped 
 * if Minimax Godmode is not enabled. If there are no moves left or the game 
 * is over, it returns the evaluation score.
 * 
 * @param board A 3x3 array representing the current Tic-Tac-Toe board.
 * @param depth The current depth in the game tree.
 * @param isMax Boolean flag indicating whether it is the maximizer's turn (bot) or the minimizer's turn (player).
 * 
 * @return The best score for the current move based on the evaluation function.
 * 
 * @see evaluate, isMovesLeft, max, min
 */
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

/**  
 * @brief Evaluates the current board state to determine if there is a winner.
 * 
 * This function checks the Tic-Tac-Toe board for winning conditions, i.e., 
 * it checks rows, columns, and diagonals for three consecutive marks (either 
 * `BOT` or `PLAYER1`). It returns a score based on the result:
 * - +10 if the `BOT` wins.
 * - -10 if `PLAYER1` wins.
 * - 0 if there is no winner yet (no winner in rows, columns, or diagonals).
 * 
 * @param b A 3x3 array representing the Tic-Tac-Toe board.
 * 
 * @return The evaluation score: 
 * - +10 for a `BOT` win,
 * - -10 for a `PLAYER1` win,
 * - 0 if there is no winner.
 * 
 * @see BOT, PLAYER1
 */
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

/**  
 * @brief Checks if there are any remaining moves on the board.
 * 
 * This function scans the Tic-Tac-Toe board for any empty cells (represented 
 * by `0`). If at least one empty cell is found, it returns true, indicating 
 * that the game can continue. If no empty cells are found, it returns false, 
 * indicating the game has ended.
 * 
 * @param board A 3x3 array representing the Tic-Tac-Toe board.
 * 
 * @return True if there are empty cells (moves left), false if the board is full.
 * 
 */
bool isMovesLeft(int board[3][3])
{
    int result;

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

    return result;
}

/**  
 * @brief Loads board states and their best moves from a file.
 * 
 * This function attempts to open a file containing saved board states and the 
 * corresponding best move for each state. If the file does not exist, a new 
 * file is created. It reads the board configurations and the best move 
 * for each board, storing them in the provided `boardStates` array.
 * 
 * Each line in the file represents one board state. The board is stored as 
 * a 3x3 grid, where 'x' denotes the BOT's move, 'o' denotes PLAYER1's move, 
 * and empty spaces are represented as ' ' (empty). The best move for each 
 * board is also saved in the file.
 * 
 * @param boardStates An array of `BoardState` structures to store the loaded board states.
 * 
 * @return The number of boards loaded from the file. If the file does not exist, 
 *         it returns 0 and creates a new file.
 * 
 * @see BoardState, FILE_BESTMOV
 */
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

/**  
 * @brief Checks if the current board configuration exists in the lookup table and updates the best move.
 * 
 * This function compares the current board with previously saved board states in the `boardStates` array. 
 * If a matching board configuration is found, it updates the provided `bestMove` structure with the best 
 * move associated with that board state. The function returns true if a match is found and the move is updated, 
 * and false if no match is found in the lookup table.
 * 
 * @param board The current Tic Tac Toe board to check against the saved states.
 * @param bestMove A pointer to the `Position` structure where the best move will be stored if a match is found.
 * @param boardStates An array of `BoardState` structures containing previously saved board configurations and their best moves.
 * @param count The number of saved board states in the `boardStates` array.
 * 
 * @return `true` if a matching board configuration is found and the best move is updated, `false` otherwise.
 * 
 * @see BoardState, Position
 */
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

/**  
 * @brief Appends the current board state and the best move to a file.
 * 
 * This function writes the current Tic Tac Toe board state to a file, encoding the board as a sequence of 
 * characters where 'o' represents Player 1, 'x' represents the Bot, and 'b' represents an empty cell. 
 * After writing the board state, it appends the best move (row and column) for the current board to the same file.
 * 
 * @param board The current Tic Tac Toe board to write to the file.
 * @param bestMove The best move to be made, represented by its row and column indices.
 * 
 * @see Position, BoardState
 */
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