#include <gtk/gtk.h>

#include <macros.h>
#include <minimax.h>
#include <ml-naive-bayes.h>

/*===============================================================================================
GLOBAL DECLARATION
===============================================================================================*/

#ifndef MAIN_H // Start of include guard
#define MAIN_H
typedef struct
{
    int pos[2]; // Array to hold row and column
} BtnPos;

struct PlayerMode
{
    char txt[2];
    int mode;
};
#endif // MAIN_H  // End of include guard

int doBOTmove();
int chkPlayerWin();
void clearBtn();
void updateScoreBtn(gpointer data);
void on_btnGrid_clicked(GtkWidget *widget, gpointer data);
void on_btnScore_clicked(GtkWidget *widget, gpointer data);
void showWin();