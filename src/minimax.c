#include <minimax.h>

int depthCounter = 0;

int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Helper function to get the minimum of two integers
int min(int a, int b)
{
    return (a < b) ? a : b;
}

struct Position findBestMove(int board[3][3])
{
    int bestVal = -1000;
    struct Position bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

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

    if (depth > 2)
        return 0;

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

bool isMovesLeft(int board[3][3]) {
    int result;
    
        __asm__ (
        "xor %%rbx, %%rbx;"          // rbx = i = 0
        "outer_loop:;"
        "cmp $3, %%ebx;"             // if i >= 3, return false
        "jge return_false;"

        "xor %%rcx, %%rcx;"          // rcx = j = 0
        "inner_loop:;"
        "cmp $3, %%ecx;"             // if j >= 3, increment i
        "jge increment_i;"

        // Calculate board[i][j]
        "mov %%rbx, %%rdx;"          // Copy i to rdx
        "imul $12, %%rdx, %%rdx;"    // rdx = i * 12 (calculate row offset)
        "add %1, %%rdx;"             // rdx = board + (i * 12), points to board[i]
        "mov (%%rdx, %%rcx, 4), %%eax;" // Load board[i][j]

        "test %%eax, %%eax;"         // Check if board[i][j] == 0
        "jz return_true;"            // If board[i][j] == 0, return true

        "inc %%rcx;"                 // Increment j
        "jmp inner_loop;"

        "increment_i:;"
        "inc %%rbx;"                 // Increment i
        "jmp outer_loop;"

        "return_false:;"
        "mov $0, %0;"                // Set result to 0 (false)
        "jmp end;"

        "return_true:;"
        "mov $1, %0;"                // Set result to 1 (true)

        "end:;"
        : "=r" (result)              // Output operand
        : "r" (board)                // Input operand
        : "rbx", "rcx", "rdx", "rax" // Clobbered registers
    );
    
    return result;
}
