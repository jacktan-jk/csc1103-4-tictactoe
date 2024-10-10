#include <stdio.h>
#include <gtk/gtk.h>
#include <stdbool.h>

#define PLAY 0x69
#define TIE 0xFF
#define WIN 0

#define SUCCESS 0
#define ERROR -1

#define PLAYER_2 0
#define PLAYER_BOT 1

/*===============================================================================================
GLOBAL DECLARATION
===============================================================================================*/

typedef struct 
{
    int pos[2]; // Array to hold row and column
} stBtnPos;

struct stPlayerMode
{
    char txt[2];
    int mode;
} ;

struct Move 
{ 
    int row, col; 
}; 

// Global variables for scores
int player1_score = 0;
int player2_score = 0;
int tie_score = 0;
int isPlayer1Turn = 1;
int board[3][3];
int gameState = PLAY;
struct stPlayerMode playerMode = {"2P", PLAYER_BOT};

GtkWidget *buttons[3][3];

int chkPlayerWin();
void clearBtn();
void updateScoreBtn(gpointer data);
void on_btnGrid_clicked(GtkWidget *widget, gpointer data);
void on_btnScore_clicked(GtkWidget *widget, gpointer data);

/*===============================================================================================
END OF GLOBAL DECLARATION
===============================================================================================*/

/*===============================================================================================
GUI FUNCTIONS
===============================================================================================*/

void clearBtn()
{
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            gtk_button_set_label(GTK_BUTTON(buttons[i][j]), ""); // Clear the button labels
            board[i][j] = 0;
        }
    }
}

void updateScoreBtn(gpointer data)
{
    // Update the score display
    char score_text[100];
    if (isPlayer1Turn == 1) {
        snprintf(score_text, sizeof(score_text),"<b>Player 1 (O): %d</b>     |     TIE: %d     |     Player 2 (X): %d     |    [%s]    ", player1_score, tie_score, player2_score, playerMode.txt);
    } 
    else 
    {
        snprintf(score_text, sizeof(score_text),"Player 1 (O): %d     |     TIE: %d     |     <b>Player 2 (X): %d</b>     |    [%s]    ", player1_score, tie_score, player2_score, playerMode.txt);
    }
    gtk_button_set_label(GTK_BUTTON(data), score_text); // Update the score button label
    gtk_label_set_markup(GTK_LABEL(gtk_bin_get_child(GTK_BIN(data))), score_text);
}

// Callback function for button clicks
void on_btnGrid_clicked(GtkWidget *widget, gpointer data) 
{
    const char *current_label = gtk_button_get_label(GTK_BUTTON(widget));
    stBtnPos *btnPos = (stBtnPos *)g_object_get_data(G_OBJECT(widget), "button-data");
    
    if(gameState != PLAY)
    {
        gameState = PLAY;
        clearBtn();
    }

    if(g_strcmp0(current_label, "") == 1)
    {
        return;
    }

    board[btnPos->pos[0]][btnPos->pos[1]] = isPlayer1Turn ? 1 : 2; // O (1), X(2)

    // Update the button text, for example, with an "O"
    gtk_button_set_label(GTK_BUTTON(widget), isPlayer1Turn ? "O" : "X");

    int retVal = chkPlayerWin();
    if(retVal == PLAY)
    {
        isPlayer1Turn = !isPlayer1Turn;
        updateScoreBtn(data);
        return;
    }

    if(retVal == WIN)
    {
        isPlayer1Turn ? player1_score++ : player2_score++;
        gameState = WIN;
    }

    if(retVal == TIE)
    {
        tie_score++;
        gameState = TIE;
    }
    updateScoreBtn(data);
}

void on_btnScore_clicked(GtkWidget *widget, gpointer data) 
{
    playerMode.mode = !playerMode.mode;
    strncpy(playerMode.txt, playerMode.mode == PLAYER_2 ? "1P" : "2P", sizeof(playerMode.txt));
    updateScoreBtn(data);
    clearBtn();
}

/*===============================================================================================
END OF GUI FUNCTIONS
===============================================================================================*/


/*===============================================================================================
LOGIC FUNCTIONS
===============================================================================================*/

int chkPlayerWin()
{
    //check both dia
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) 
    {
        return WIN;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0) 
    {
        return WIN;
    }

    //check rows and col
    for (int i = 0; i < 3; i++) 
    {
        // Check rows
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0) 
        {
            return WIN;
        }
        // Check columns
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0) 
        {
            return WIN;
        }
    }

    //check for unclicked grid, if none left then tie
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (board[i][j] == 0) 
            {
                return PLAY;    
            }
        }
    }

    return TIE;
}

struct Move findBestMove(char board[3][3]) 
{ 
    int bestVal = -1000; 
    struct Move bestMove; 
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
            if (board[i][j]==0) 
            { 
                // Make the move 
                board[i][j] = 3; 
  
                // compute evaluation function for this 
                // move. 
                int moveVal = minimax(board, 0, false); 
  
                // Undo the move 
                board[i][j] = 0; 
  
                // If the value of the current move is 
                // more than the best value, then update 
                // best/ 
                if (moveVal > bestVal) 
                { 
                    bestMove.row = i; 
                    bestMove.col = j; 
                    bestVal = moveVal; 
                } 
            } 
        } 
    } 
  
    printf("The value of the best Move is : %d\n\n", 
            bestVal); 
  
    return bestMove; 
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
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *score_button;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tic-Tac-Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 800);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a box to hold the grid and score button with padding
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(box), 50); // Set padding

    // Create a grid to hold the buttons
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
        "button:pressed { background-color: darkgray; }\n", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Create a button for the score display
    score_button = gtk_button_new_with_label("");
    gtk_label_set_markup(GTK_LABEL(gtk_bin_get_child(GTK_BIN(score_button))), "<b>Player 1 (O): 0</b>     |     TIE: 0     |     Player 2 (X): 0     |    [2P]    "); 
    g_signal_connect(score_button, "clicked", G_CALLBACK(on_btnScore_clicked), score_button);
    gtk_grid_attach(GTK_GRID(grid), score_button, 0, 3, 3, 1); // Attach score button below the grid

    // Create the 9 buttons and add them to the grid
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            buttons[i][j] = gtk_button_new_with_label("");
            
            stBtnPos *data = g_new(stBtnPos, 1); // Allocate memory for the structure
            data->pos[0] = i; // Store row
            data->pos[1] = j; // Store column
            
            // Set the structure as data on the button
            g_object_set_data(G_OBJECT(buttons[i][j]), "button-data", data);

            g_signal_connect(buttons[i][j], "clicked", G_CALLBACK(on_btnGrid_clicked), score_button); // Pass score_button as data
            gtk_grid_attach(GTK_GRID(grid), buttons[i][j], j, i, 1, 1); // Attach buttons to the grid
        }
    }

    // Make the buttons expand to fill the available space
    for (int i = 0; i < 3; i++) {
        gtk_widget_set_vexpand(buttons[i][0], TRUE);
        gtk_widget_set_hexpand(buttons[i][0], TRUE);
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