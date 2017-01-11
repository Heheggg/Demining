#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define BOARD_SIZE 16;
#define TRUE 1;
#define FALSE 0;

char board[BOARD_SIZE][BOARD_SIZE];
char game_board[BOARD_SIZE][BOARD_SIZE];

int victory = FALSE;

void display();
void build_board();
void build_gboard();
void place_mines();
void print();
void print_full();
void start();
int play_game();
void play_again();
int check_win_game();
void check_for_mine(int, int);
int check_for_nearby_mines(int, int);
