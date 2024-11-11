#include <main.h>

/*===============================================================================================
GLOBAL DECLARATION
===============================================================================================*/

// Global variables for scores
int iPlayer1_score = 0;
int iPlayer2_score = 0;
int iTie_score = 0;
int iBoard[3][3];
int iGameState = PLAY;
int iWinPos[3][3];

bool isPlayer1Turn = true;
bool isMLAvail = true;

struct stPlayerMode playerMode = {"2P", MODE_2P};

GtkWidget *btnGrid[3][3];

/*===============================================================================================
END OF GLOBAL DECLARATION
===============================================================================================*/

/*===============================================================================================
GUI FUNCTIONS
===============================================================================================*/

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

// Callback function for button clicks
void on_btnGrid_clicked(GtkWidget *widget, gpointer data)
{
    const char *current_label = gtk_button_get_label(GTK_BUTTON(widget));
    stBtnPos *btnPos = (stBtnPos *)g_object_get_data(G_OBJECT(widget), "button-data");

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
        PRINT_DEBUG("[DEBUG] GAME RESULT -> %s Win\n", isPlayer1Turn ? "Player 1": playerMode.mode == MODE_2P ? "Player 2" : "BOT");
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

    updateScoreBtn(data);
}

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
    updateScoreBtn(data);
    clearBtn();
}

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
int doBOTmove()
{
    struct Position botMove;
    if (playerMode.mode == MODE_MM)
    {
        struct timeval t;
        double time1
        double time2;
        gettimeofday(&t, NULL);
        time1 = t.tv_sec + 1.0e-6 * t.tv_usec;

        if (rand() % 100 < 70)
        {
            botMove = findBestMove(iBoard);
        }
        else
        {
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
        }
        gettimeofday(&t, NULL);
        time2 = t.tv_sec + 1.0e-6 * t.tv_usec;
        PRINT_DEBUG("Minimax Elapsed: %f seconds \n\n", (double)(time2 - time1));
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

            stBtnPos *data = g_new(stBtnPos, 1); // Allocate memory for the structure
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
