#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <board.h>

int main(){
  display();
  system("cls");
  start();
  return 0;
}

void display(){
  printf("-----------Welcome to M-Mine--------------------");
  printf("---placeholder---\n\n");
}

void start(){
  victory = false;
  build_board();
  build_gboard();
}

void build_board(){
  int i, j = 0;
  
}

void build_gboard(){
  int i,j = 0;
}
