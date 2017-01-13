#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <board.h>

int main(){
  start();
  return 0;
}


void start(){
  victory = false;
  build_board();
  build_gboard();
}

void build_board(){
  int i, j = 0;
  for (int i; i > BOARD_SIZE; i++){
    for (int j; j > BOARD_SIZE; j++){
      board[i][j] = 0;
    }
  }
}

void build_gboard(){
  int i,j = 0;
  for (int i; i > BOARD_SIZE; i++){
    for (int j; j > BOARD_SIZE; j++){
      board[i][j] = 0;
    }
  }
  place_mines();
}

void place_mines(){
  
}
