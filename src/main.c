#include <main.h>

/*===============================================================================================
GLOBAL DECLARATION
===============================================================================================*/

/**  
 * @var int iPlayer1_score
 * @brief Global variable to track Player 1's score.
 * 
 * @var int iPlayer2_score
 * @brief Global variable to track Player 2's or Bot's (Minimax/ML) score.
 * 
 * @var int iTie_score
 * @brief Global variable to track the number of ties/draws.
 * 
 * @var int iBoard[3][3]
 * @brief Global 2D array representing the Tic-Tac-Toe game board.
 * 
 * @var int iWinPos[3][3]
 * @brief Global 2D array to track winning positions on the board.
 * 
 * @var int iGameState
 * @brief Global variable to track the current game state.
 * 
 * Game states:
 * - PLAY: The game is ongoing.
 * - TIE: The game ended in a tie.
 * - WIN: A player has won the game.
 * 
 * @var bool isPlayer1Turn
 * @brief Global flag indicating if it's Player 1's turn.
 * 
 * @var bool isMLAvail
 * @brief Global flag indicating if Machine Learning mode is available.
 * This is set to false if the ML data file is missing, disabling the ML game mode.
 * 
 * @var struct PlayerMode playerMode
 * @brief Global structure to track the current game mode.
 * 
 * Fields:
 * - txt: Text representation of the current mode (e.g., "2P", "MM", "ML").
 * - mode: Integer value representing the current game mode.
 * 
 * Player modes:
 * - MODE_2P: Player vs Player mode.
 * - MODE_MM: Minimax Bot mode.
 * - MODE_ML: Machine Learning Bot mode.
 * 
 * @var GtkWidget *btnGrid[3][3]
 * @brief Global 2D array of buttons corresponding to the game grid.
 */
int iPlayer1_score = 0;
int iPlayer2_score = 0;
int iTie_score = 0;
int iGameState = PLAY;
int iBoard[3][3];
int iWinPos[3][3];

bool isPlayer1Turn = true;
bool isMLAvail = true;

struct PlayerMode playerMode = {"2P", MODE_2P};

GtkWidget *btnGrid[3][3];

/*===============================================================================================
END OF GLOBAL DECLARATION
===============================================================================================*/

/*===============================================================================================
GUI FUNCTIONS
===============================================================================================*/

/** 
 * @brief Clears the game board and resets the player's turn.
 * 
 * This function is used to reset the game board for a new round. It clears the labels
 * on the buttons in the grid and resets the internal board state (iBoard) to 0. It also
 * sets the player turn back to player 1.
 * 
 * @details
 * - Sets all button labels in the `btnGrid` to an empty string.
 * - Resets all values in the `iBoard` array to 0, indicating no moves.
 * - Resets `isPlayer1Turn` to `true`, indicating it’s Player 1's turn.
 * 
 * @see iBoard
 * @see btnGrid
 * @see isPlayer1Turn
 */
void clearBtn()
{
    isPlayer1Turn = true;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gtk_button_set_label(GTK_BUTTON(btnGrid[i][j]), ""); // Clear the button labels
            iBoard[i][j] = 0;
        }
    }
}

/** 
 * @brief Updates the score display on the button.
 * 
 * This function updates the label on a score button to display the current scores
 * for Player 1, Player 2, and Ties. It changes the text formatting depending on
 * which player's turn it is, highlighting the active player.
 * 
 * @param data A gpointer (usually a button widget) that is used to update the label.
 * 
 * @details
 * - The function checks if it's Player 1's turn and updates the score display with a bold label for Player 1, or Player 2’s turn with Player 2's score in bold.
 * - The button text is updated using `gtk_button_set_label()`, and the label markup is updated using `gtk_label_set_markup()`.
 * - The score includes Player 1's score, Player 2's score, the tie count, and the current game mode (`playerMode.txt`).
 * 
 * @see iPlayer1_score, iTie_score, iPlayer2_score, playerMode
 */
void updateScoreBtn(gpointer data)
{
    // Update the score display
    char score_text[100];
    if (isPlayer1Turn == true)
    {
        snprintf(score_text, sizeof(score_text), "<b>Player 1 (O): %d</b>   |   TIE: %d   |   Player 2 (X): %d   |  [%s]  ", iPlayer1_score, iTie_score, iPlayer2_score, playerMode.txt);
    }
    else
    {
        snprintf(score_text, sizeof(score_text), "Player 1 (O): %d   |   TIE: %d   |   <b>Player 2 (X): %d</b>   |  [%s]  ", iPlayer1_score, iTie_score, iPlayer2_score, playerMode.txt);
    }
    gtk_button_set_label(GTK_BUTTON(data), score_text); // Update the score button label
    gtk_label_set_markup(GTK_LABEL(gtk_bin_get_child(GTK_BIN(data))), score_text);
}

/** 
 * @brief Callback function for handling button clicks on the game grid.
 * 
 * This function handles the logic for a player’s move when a button in the game grid is clicked.
 * It updates the game state, checks for a winner or tie, and updates the score display. It also handles
 * player turns, Bot moves (if applicable), and resets the game board when the game state changes.
 * 
 * @param widget The GtkWidget that was clicked (the button in the grid).
 * @param data Additional data passed to the callback (usually the score display data).
 * 
 * @details
 * - If the game state is not `PLAY`, the game will be reset, and the score updated.
 * - If the clicked button already has a label, the function returns early (no action is taken).
 * - If the clicked button is empty, the move is recorded in the `iBoard` array (Player 1 or Bot).
 * - After each move, the game checks for a win or tie condition using `chkPlayerWin()`.
 * - If Player 1 or Player 2 wins, the score is updated, and the win condition is shown.
 * - If the game ends in a tie, the tie score is updated.
 * - If the game is in **2P** mode, turns alternate between Player 1 and Player 2.
 * - In **Bot mode**, the Bot will automatically make a move after Player 1’s turn.
 * - In **ML mode**, the dataset is re-read and initialized after the game ends.
 * 
 * @see iBoard, isPlayer1Turn, iPlayer1_score, iPlayer2_score, iTie_score
 * @see playerMode, updateScoreBtn, chkPlayerWin, doBOTmove, showWin
 * @see PLAY, TIE, WIN
 */
// Callback function for button clicks
void on_btnGrid_clicked(GtkWidget *widget, gpointer data)
{
    const char *current_label = gtk_button_get_label(GTK_BUTTON(widget));
    BtnPos *btnPos = (BtnPos *)g_object_get_data(G_OBJECT(widget), "button-data");

    if (iGameState != PLAY)
    {
        iGameState = PLAY;
        clearBtn();
        updateScoreBtn(data);
        return;
    }

    if (strcmp(current_label, "") != 0)
    {
        return;
    }

    iBoard[btnPos->pos[0]][btnPos->pos[1]] = isPlayer1Turn ? PLAYER1 : BOT; // O (1), X(2), BOT is the same as player 2

    // Update the button text, for example, with an "O"
    gtk_button_set_label(GTK_BUTTON(widget), isPlayer1Turn ? "O" : "X");

    int retVal = chkPlayerWin();

    if (retVal == PLAY)
    {
        isPlayer1Turn = !isPlayer1Turn;
        updateScoreBtn(data);

        if (playerMode.mode == MODE_2P)
        {
            return;
        }

        doBOTmove();
        retVal = chkPlayerWin();
    }

    if (retVal == WIN)
    {
        showWin();
        PRINT_DEBUG("[DEBUG] GAME RESULT -> %s Win\n", isPlayer1Turn ? "Player 1" : playerMode.mode == MODE_2P ? "Player 2"
                                                                                                               : "BOT");
        isPlayer1Turn ? iPlayer1_score++ : iPlayer2_score++;
        iGameState = WIN;
    }

    if (retVal == TIE)
    {
        PRINT_DEBUG("[DEBUG] GAME RESULT -> TIE\n");
        iTie_score++;
        iGameState = TIE;
    }

    if (playerMode.mode != MODE_2P)
    {
        isPlayer1Turn = !isPlayer1Turn;
    }

    if (isMLAvail && playerMode.mode == MODE_ML)
    {
        if (retVal == WIN || retVal == TIE)
        {
            readDataset(RES_PATH "" DATA_PATH, true);
            initData();
        }
    }
    updateScoreBtn(data);
}

/** 
 * @brief Handles button click for score.
 * 
 * Toggles the player mode and updates the displayed score.
 * 
 * @param widget The widget that triggered the event.
 * @param data Additional data passed to the callback.
 * 
 * @see playerMode, isMLAvail, isPlayer1Turn, updateScoreBtn, clearBtn
 */
void on_btnScore_clicked(GtkWidget *widget, gpointer data)
{
    playerMode.mode = (playerMode.mode > 1 ? MODE_2P : ++playerMode.mode);
    switch (playerMode.mode)
    {
    case MODE_MM:
        strncpy(playerMode.txt, "MM", sizeof(playerMode.txt));
        break;

    case MODE_ML:
        if (isMLAvail)
        {
            strncpy(playerMode.txt, "ML", sizeof(playerMode.txt));
            break;
        }

    default:
        playerMode.mode = MODE_2P;
        strncpy(playerMode.txt, "2P", sizeof(playerMode.txt));
    }
    PRINT_DEBUG("playerMode: %d\n", playerMode.mode);
    isPlayer1Turn = true;
    iPlayer1_score = iPlayer2_score = iTie_score = 0;
    
    clearBtn();
    updateScoreBtn(data);
}

/** 
 * @brief Clears the winning positions and resets the grid.
 * 
 * Iterates over the win positions and clears any displayed labels,
 * resetting the grid to its initial state.
 * 
 * @see iWinPos, btnGrid
 */
void showWin()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (iWinPos[i][j] != WIN)
            {
                gtk_button_set_label(GTK_BUTTON(btnGrid[i][j]), "");
            }
        }
    }
    memset(iWinPos, 0, sizeof(iWinPos));
}
/*===============================================================================================
END OF GUI FUNCTIONS
===============================================================================================*/

/*===============================================================================================
LOGIC FUNCTIONS
===============================================================================================*/

/** 
 * @brief Executes the bot's move based on the current game mode.
 * 
 * In MM mode:
 * - The bot performs a minimax move by default.
 * - If the minimax move is not chosen, the bot randomly selects a position.
 * 
 * In ML mode, the bot uses machine learning to determine the best position.
 * 
 * The function also measures and logs the time taken for the minimax move.
 * 
 * @return SUCCESS if the bot's move was made successfully.
 * @see playerMode, isMLAvail, iBoard, findBestMove, getBestPosition, btnGrid
 */
int doBOTmove()
{
    struct Position botMove;
    if (playerMode.mode == MODE_MM)
    {
        startElapseTime();
#if !(MINIMAX_GODMODE)
        if (rand() % 100 < 80)
#endif
        {
            botMove = findBestMove(iBoard);
        }
#if !(MINIMAX_GODMODE)
        else
        {
            startElapseTime();
            int randRow = rand() % 3;
            int randCol = rand() % 3;
            bool bIsDone = false;

            while (!bIsDone)
            {
                if (iBoard[randRow][randCol] == EMPTY)
                {
                    PRINT_DEBUG("Random Move -> R:%d C:%d\n", randRow, randCol);
                    botMove.row = randRow;
                    botMove.col = randCol;
                    bIsDone = !bIsDone;
                }
                else
                {
                    randRow = rand() % 3;
                    randCol = rand() % 3;
                }
            }
            stopElapseTime("Minimax Random Move");
        }
#endif
        stopElapseTime("Minimax Move");
    }
    else // ML mode, sets ML as default if for some reason playermode.mode has expected value.
    {
        if (isMLAvail)
        {
            botMove = getBestPosition(iBoard, 'x');
        }
    }

    iBoard[botMove.row][botMove.col] = BOT;
    gtk_button_set_label(GTK_BUTTON(btnGrid[botMove.row][botMove.col]), "X");
    return SUCCESS;
}

/** 
 * @brief Checks the current game board for a win or tie.
 * 
 * This function checks all possible win conditions:
 * - Diagonals
 * - Rows
 * - Columns
 * 
 * If there is a winning line, it marks the winning positions and returns WIN.
 * If there are no winning conditions and the board is full, it returns TIE.
 * If there are unclicked positions left, it returns PLAY.
 * 
 * @return WIN if there is a winner, TIE if the game is a tie, PLAY if the game is still ongoing.
 * @see iBoard, iWinPos
 */
int chkPlayerWin()
{
    // check both dia
    if (iBoard[0][0] == iBoard[1][1] && iBoard[1][1] == iBoard[2][2] && iBoard[0][0] != 0)
    {
        iWinPos[0][0] = iWinPos[1][1] = iWinPos[2][2] = WIN;
        return WIN;
    }

    if (iBoard[0][2] == iBoard[1][1] && iBoard[1][1] == iBoard[2][0] && iBoard[0][2] != 0)
    {
        iWinPos[0][2] = iWinPos[1][1] = iWinPos[2][0] = WIN;
        return WIN;
    }

    // check rows and col
    for (int i = 0; i < 3; i++)
    {
        // Check rows
        if (iBoard[i][0] == iBoard[i][1] && iBoard[i][1] == iBoard[i][2] && iBoard[i][0] != 0)
        {
            iWinPos[i][0] = iWinPos[i][1] = iWinPos[i][2] = WIN;
            return WIN;
        }
        // Check columns
        if (iBoard[0][i] == iBoard[1][i] && iBoard[1][i] == iBoard[2][i] && iBoard[0][i] != 0)
        {
            iWinPos[0][i] = iWinPos[1][i] = iWinPos[2][i] = WIN;
            return WIN;
        }
    }

    // check for unclicked grid, if none left then tie
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (iBoard[i][j] == 0)
            {
                return PLAY;
            }
        }
    }

    return TIE;
}

/*===============================================================================================
END OF LOGIC FUNCTIONS
===============================================================================================*/

/*===============================================================================================
MAIN
*Init GUI interface and global variable/objects
===============================================================================================*/

/** 
 * @brief Initializes and runs the Tic-Tac-Toe GTK application.
 * 
 * This function initializes GTK, creates the main window, and sets up the game grid,
 * score display, and buttons. It also handles the setup for the game mode and ML availability.
 * The game board and score are displayed, and event listeners are attached to buttons.
 * 
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return SUCCESS if the program runs successfully.
 * @see initData, on_btnScore_clicked, on_btnGrid_clicked, btnGrid, score_button
 */
int main(int argc, char *argv[])
{
    int retVal = SUCCESS;
    srand(time(NULL));

    retVal = initData();
    if (retVal != SUCCESS) // disable ML
    {
        isMLAvail = false;
    }

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *score_button;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tic-Tac-Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 950);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a box to hold the grid and score button with padding
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(box), 50); // Set padding

    // Create a grid to hold the btnGrid
    grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0); // Add grid to the box

    // Set the grid to expand
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    // Set CSS styles
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
                                    "window { background-color: black; }\n"
                                    "button { background-color: black; color: white; border: 3px solid white; font-size: 24px; background-image: none; }\n"
                                    "button:pressed { background-color: darkgray; }\n",
                                    -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Create a button for the score display
    score_button = gtk_button_new_with_label("");
    gtk_label_set_markup(GTK_LABEL(gtk_bin_get_child(GTK_BIN(score_button))), "<b>Player 1 (O): 0</b>   |   TIE: 0   |   Player 2 (X): 0   |  [2P]  ");
    g_signal_connect(score_button, "clicked", G_CALLBACK(on_btnScore_clicked), score_button);
    gtk_grid_attach(GTK_GRID(grid), score_button, 0, 3, 3, 1); // Attach score button below the grid

    // Create the 9 btnGrid and add them to the grid
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            btnGrid[i][j] = gtk_button_new_with_label("");

            BtnPos *data = g_new(BtnPos, 1); // Allocate memory for the structure
            data->pos[0] = i;                    // Store row
            data->pos[1] = j;                    // Store column

            // Set the structure as data on the button
            g_object_set_data(G_OBJECT(btnGrid[i][j]), "button-data", data);

            g_signal_connect(btnGrid[i][j], "clicked", G_CALLBACK(on_btnGrid_clicked), score_button); // Pass score_button as data
            gtk_grid_attach(GTK_GRID(grid), btnGrid[i][j], j, i, 1, 1);                               // Attach btnGrid to the grid
        }
    }

    // Make the btnGrid expand to fill the available space
    for (int i = 0; i < 3; i++)
    {
        gtk_widget_set_vexpand(btnGrid[i][0], TRUE);
        gtk_widget_set_hexpand(btnGrid[i][0], TRUE);
    }
    gtk_widget_set_vexpand(score_button, TRUE);
    gtk_widget_set_hexpand(score_button, TRUE);

    // Add the box to the window
    gtk_container_add(GTK_CONTAINER(window), box);

    // Show everything
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return SUCCESS;
}
/*
===============================================================================================
END OF MAIN
===============================================================================================
*/
