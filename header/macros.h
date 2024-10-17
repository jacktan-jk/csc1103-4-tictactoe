#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

#define DEBUG 1

#if DEBUG
#define PRINT_DEBUG(...) printf(__VA_ARGS__);
#else
#define PRINT_DEBUG(...)
#endif