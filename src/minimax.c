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

// bool isMovesLeft(int board[3][3])
// {
//     for (int i = 0; i < 3; i++)
//         for (int j = 0; j < 3; j++)
//             if (board[i][j] == EMPTY)
//                 return true;
//     return false;
// }

bool isMovesLeft(int board[3][3])
{
    int result;

    // Inline assembly for the function
    __asm__(
        "PUSH {lr} \n"                  // Save lr to the stack
        "MOV R2, #0 \n"                 // Initialising the outer loop counter (i=0)

        "outer_loop: \n"
        "CMP R2, #3 \n"                 // Compare i with 3
        "BGE return_false \n"           // if i >= 3, branch to return false

        "MOV R3, #0 \n"                 // Initialise the inner loop counter

        "inner_loop: \n"            
        "CMP R3, #3 \n"                 // Compare j with 3
        "BGE increament_i \n"           // If j >= 3, increment i

        // Calculate the address of board[i][j]
        "LDR R4, [R0, R2, LSL #2] \n"   // Load board[i] address
        "LDR R5, [R4, R3, LSL #2] \n"   // Load board[i][j]

        "CMP R5, #0 \n"                 // Compare board[i][j] with EMPTY (assuming EMPTY = 0)
        "BEQ return_true \n"            // If it is empty, return true

        "ADD R3, R3, #1 \n"             // Increment j
        "B inner_loop \n"               // Repeat inner loop

        "increment_i: \n"       
        "ADD R2, R2, #1 \n"             // Increment i
        "B outer_loop \n"               // Repeat outer loop

        "return_false: \n"
        "MOV R0, #0 \n"                 // Return 0 (false)

        "POP {lr} \n"                   // Restore lr from the stack
        "BX lr \n"                      // Return from function

        "return_true: \n"
        "MOV R0, #1 \n"                 // Return 1 (true)
        "POP {lr} \n"                   // Restore lr from the stack
        "BX lr \n"                      // Return from function

        : "=r"(result)           // Output the result into the 'result' variable
        : "r"(board)             // Input the board array as an argument
        : "r2", "r3", "r4", "r5" // Clobbered registers (R2, R3, R4, R5)SFF
    );

    return result;
}
