#include <stdio.h>
#include <gtk/gtk.h>
#include <stdbool.h>

#define PLAY 0x69
#define TIE 0xFF
#define WIN 0

#define SUCCESS 0
#define ERROR -1

#define MODE_2P 0
#define MODE_BOT 1

#define EMPTY 0
#define PLAYER1 1
#define BOT 2       //this is also used for player 2

/*===============================================================================================
GLOBAL DECLARATION
===============================================================================================*/

#ifndef MAIN_H  // Start of include guard
#define MAIN_H
typedef struct 
{
    int pos[2]; // Array to hold row and column
} stBtnPos;

struct stPlayerMode
{
    char txt[2];
    int mode;
} ;
#endif // MAIN_H  // End of include guard

int chkPlayerWin();
void clearBtn();
void updateScoreBtn(gpointer data);
void on_btnGrid_clicked(GtkWidget *widget, gpointer data);
void on_btnScore_clicked(GtkWidget *widget, gpointer data);