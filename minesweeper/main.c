#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "mine_sweeper.h"

int main(int argc, char* argv[]){

  Board* board = NULL;
  board = (Board*)malloc(sizeof(Board));
  int user_x = 0;
  int user_y = 0;
  int user_play = 0;
 
 

  read_args(board, argc, argv);
  create_board(board);
  player_board(board);
  place_mines(board);
  play_game(board, user_x, user_y, user_play);
  
 

 

 
  return 0;
}
