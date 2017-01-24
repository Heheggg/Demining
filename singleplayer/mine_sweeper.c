#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "mine_sweeper.h"

 
void read_args(Board* board, int argc, char** argv){ //read in arguments

 
 if(argc > 5){
   printf("Too many arguments. Usage:\n./mine_sweeper.out num_rows num_cols num_mines [seed])");
   exit(0);
 }

 else if(argc < 4){
   printf("Not enough arguments. Usage:\n./mine_sweeper.out num_rows num_cols num_mines [seed])");
   exit(0);
 }

 board->rows = atoi(argv[1]); //assign variables in struct to each command line arg
 board->cols = atoi(argv[2]);
 board->mines = atoi(argv[3]);
 board->argc = argc;
 if(argc == 5){
   srand(atoi(argv[4])); //place the seed
 }
 else{
   srand(time(0));
 }
 

 

  return;
  }

void create_board(Board* board){
  int i = 0;
  int j = 0;

 
  board->tiles = (char**)malloc(board->rows * sizeof(char *)); //create space for tiles board
  for(i = 0; i  < board->rows; i++){
    board->tiles[i] = (char*)malloc(board->cols * sizeof(char));
    
  }
  for(i = 0; i < board->rows; i++){
    for(j = 0; j < board->cols; j++){
      board->tiles[i][j] = '#';
    }
  }
  
}

void player_board(Board* board){
  int i = 0;
  int j = 0;

  board->player_board = (char**)malloc(board->rows * sizeof(char *)); //create space for player board
  for(i = 0; i  < board->rows; i++){
    board->player_board[i] = (char*)malloc(board->cols * sizeof(char));
    
  }
 
  for(i = 0; i < board->rows; i++){
    for(j = 0; j < board->cols; j++){
      board->player_board[i][j] = '0';
    }
  }
  
}


void destroy_board(Board* board){ //destroy board
  int i = 0;

  for(i = 0; i < board->rows; i++){ //free memory
    free(board->tiles[i]);
  }
  free(board->tiles);

  for(i = 0; i < board->rows; i++){
    free(board->player_board[i]);
  }
  free(board->player_board);
  
  

  return;
}

void place_mines(Board* board){
  int i = 0;
  int r = 0;
  int c = 0;
  
  if(board->argc == 5){ //if there is enough args for a seed, place seed
    while(i < board->mines){
      r = (rand()%(board->rows));
      c = (rand()%(board->cols));
      if(board->player_board[(board->rows)-r-1][c]!= '*'){ //board->rows-r-1 to place upside down
	printf("Placing mine at %d, %d\n", r, c);
	board->player_board[(board->rows)-r-1][c] = '*';
	i++;
      }
      else{
	continue;
      }
    }
  }
  else{ //else automatically create seed

     while(i < board->mines){
       r = (rand()%(board->rows));
       c = (rand()%(board->cols));
       if(board->player_board[(board->rows)-r-1][c]!= '*'){
	 printf("Placing mine at %d, %d\n", r, c);
	 board->player_board[(board->rows)-r-1][c] = '*';
	
	i++;
      }
      else{
	continue;
      }
     }
  }
 



}

void play_game(Board* board, int user_x, int user_y, int user_play){

  do{ //while game is not over/won, play the game
    num_bombs_left(board);
    print_board(board);
    place_numbers(board);
    user_input(board, &user_x, &user_y, user_play);
    valid_play(user_x, user_y, board);
    check_play(user_x, user_y, board, user_play);
   
   
    
    
  }

  while(game_won(board) == false);

    return;
    
    }


  
bool game_won(Board* board){
  int i, j = 0;
  int marked = 0; //check that all the tiles are uncovered and all the mines are marked
 
   for(i = 0; i < board->rows; i++){
    for(j = 0; j < board->cols; j++){
      if(board->tiles[i][j] == '#' || board->tiles[i][j] == '?'){
	return false;
      }
      else{
	if(board->player_board[i][j] == '*' && board->tiles[i][j] == '!'){
	  marked++; //increment mark counter
	}
     
        else{
	  marked = 0;
	}
      }
    }

   }
  
   if(marked == board->mines){
    print_playboard(board);
    printf("You Won!!\n");
    destroy_board(board);
    exit(0);
    return true;
   }
  
   

    
  

  return false;
}
  


void num_bombs_left(Board* board){
  printf("There are %d mines left\n", board->mines); //print how many bombs are left 

  return;
}

void user_input(Board* board, int* user_x, int* user_y, int user_play){
  bool a = false;

  while(a == false){ //check valid play each time
    
  printf("Enter row a row between 0-%d and a column between 0-%d: ", board->rows-1, board->cols-1);
  scanf("%d %d", user_x, user_y);
  a = valid_play(*user_x, *user_y, board);
  }
  *user_x = (board->rows) - 1 - *user_x;

 
  
  if(board->tiles[*user_x][*user_y] == '?'){ //check if question and possible choices
    printf("Enter Action\n0. UnQuestion\n1. Cancel\nAction: ");
    scanf("%d", &user_play);
     if(user_play == 0){
       board->tiles[*user_x][*user_y] = '#';
     }
     else{
      printf("Enter row a row between 0-%d and a column between 0-%d: ", board->rows-1, board->cols-1);
      scanf("%d %d", user_x, user_y);
      *user_x = (board->rows) - 1 - *user_x;
      while(valid_play(*user_x, *user_y, board) == false){
	printf("Enter row a row between 0-%d and a column between 0-%d: ", board->rows-1, board->cols-1);
	scanf("%d %d", user_x, user_y);
	*user_x = (board->rows) - 1 - *user_x;
      }
	
     
       
     }
       
    num_bombs_left(board);
    print_board(board);
    printf("Enter row a row between 0-%d and a column between 0-%d: ", board->rows-1, board->cols-1);
    scanf("%d %d", user_x, user_y);
    *user_x = (board->rows) - 1 - *user_x;
  }
  else if(board->tiles[*user_x][*user_y] == '!'){ //check if marked and possible choices
     printf("Enter Action\n0. UnMark\n1. Cancel\nAction: ");
     scanf("%d", &user_play);
     if(user_play == 0){
       board->mines++;
       board->tiles[*user_x][*user_y] = '#';
     }
     else{
       printf("Enter row a row between 0-%d and a column between 0-%d: ", board->rows-1, board->cols-1);
       scanf("%d %d", user_x, user_y);
       valid_play(*user_x, *user_y, board);
       *user_x = (board->rows) - 1 - *user_x;
       while(valid_play(*user_x, *user_y, board) == false){
	printf("Enter row a row between 0-%d and a column between 0-%d: ", board->rows-1, board->cols-1);
	scanf("%d %d", user_x, user_y);
	*user_x = (board->rows) - 1 - *user_x;
      }
      
       
     }
     
     num_bombs_left(board);
     print_board(board);
     printf("Enter row a row between 0-%d and a column between 0-%d: ", board->rows-1, board->cols-1);

  scanf("%d %d", user_x, user_y);
  *user_x = (board->rows) - 1 - *user_x;
  }
  



    return;

  }

void menu_input(int* user_play, Board* board, int user_x, int user_y){
  printf("Enter Action\n0. Reveal\n1. Question\n2. Mark\n3. Cancel\nAction: ");
  scanf("%d", user_play);
  menu_ops(board, *user_play, user_x, user_y); //get menu input and check options

  return;
}


int menu_ops(Board* board, int user_play, int user_x, int user_y){
  if(user_play > 3 || user_play < 0){ //make sure user input is valid for menu ops
    user_input(board, &user_x, &user_y, user_play);
    menu_input(&user_play, board, user_x, user_y);
  }
  else{
    
  if(user_play == 0){
    reveal(user_x, user_y, board);
  }
  else if(user_play == 1){
    question(board, user_x, user_y, &user_play);
  }
  else if(user_play == 2){
    marked(board, user_x, user_y, &user_play);
  }
  else if(user_play == 3){
    user_input(board, &user_x, &user_y, user_play);
  }
  }
  
    

  return 0;
}

void question(Board* board, int user_x, int user_y, int* user_play){ //checks for question bomb
  if(board->tiles[user_x][user_y] == '#'){
    board->tiles[user_x][user_y] = '?';
  }
  else if(board->tiles[user_x][user_y] == '?'){
    printf("Enter Action\n0. UnQuestion\n1. Cancel\nAction: ");
    scanf("%d", user_play);
    if(*user_play == 0){
      board->tiles[user_x][user_y] = '#';
    }
    else{
      user_input(board, &user_x, &user_y, *user_play);
    }
  }
  return;
}

 
void marked(Board* board, int user_x, int user_y, int* user_play){ //check marked bomb
  if(board->tiles[user_x][user_y] == '#'){
    board->tiles[user_x][user_y] = '!';
    board->mines--;

   
  }
  else if(board->tiles[user_x][user_y] == '!'){
    printf("Enter Action\n0. UnMark\n1. Cancel\nAction: ");
    scanf("%d", user_play);
    if(*user_play == 0){
      board->tiles[user_x][user_y] = '#';
      board->mines++;
     
    }
    else{
      user_input(board, &user_x, &user_y, *user_play);
    }
  }
    

  return;
}

  



void print_board(Board* board){ //print the tiles board that the user sees
  int i, j;

  for(i = 0; i < board->rows; i++){
    printf("%d ", (board->rows)-i-1);
    for(j = 0; j < board->cols; j++){
      printf("%c ", board->tiles[i][j]);
    }
    printf("\n");
  }
  printf("  ");
  for(i = 0; i < board->cols; i++){
    printf("%d ", i);
  }
  printf("\n");
  return;
}


void print_playboard(Board* board){ //print player board, which isn't actually displayed but used for testing
  int i, j;

  for(i = 0; i < board->rows; i++){
    printf("%d ", (board->rows)-i-1);
    for(j = 0; j <board->cols; j++){
      printf("%c ", board->player_board[i][j]);
    }
    printf("\n");
  }
  printf("  ");
  for(i = 0; i < board->cols; i++){
    printf("%d ", i);
  }
  printf("\n");
  return;
}



bool valid_play(int user_x, int user_y, Board* board){ //check that user input rows and columns are valid

   if(user_x >= board->rows || user_y >= board->cols || user_x < 0 || user_y < 0){
      return false;
   }
   else{
      return true;
   }
}
  
void check_play(int user_x, int user_y, Board* board, int user_play){ //check what to do for each visibility based on user input
  if(board->tiles[user_x][user_y] == '#'){
    menu_input(&user_play, board, user_x, user_y);
  }
  else if(board->tiles[user_x][user_y] == '?'){
    question(board, user_x, user_y, &user_play);
  }
  else if(board->tiles[user_x][user_y] == '!'){
    marked(board, user_x, user_y, &user_play);
  }
  else{
    printf("This tile is already revealed.\n");
    user_input(board, &user_x, &user_y, user_play);
    menu_input(&user_play, board, user_x, user_y);
  }

  return;
}



void reveal(int user_x, int user_y, Board* board){ //reveal tiles with different base cases
  int i = 0;
  int j = 0;

  if(user_x < 0 || user_x >= board->rows || user_y < 0 || user_y >= board->cols){
    return;
  }
  else{
    if(board->player_board[user_x][user_y] == '*'){
    print_playboard(board);
    printf("You Lost :(\n");
    destroy_board(board);
    exit(0);
    return;
   
    }
 
 
    else if(board->tiles[user_x][user_y] == '?'){ //if there is a question mark, don't do anything
     
      return;
    }
    else if(board->tiles[user_x][user_y] == '!'){ //if tile is marked, don't do anything
      
      return;
    }
  
    else if(board->player_board[user_x][user_y] > '0'){ //if there is a number on the board, just reveal that number
      board->tiles[user_x][user_y] = board->player_board[user_x][user_y];
   
    }
    else if(board->tiles[user_x][user_y] > '0'){
      return;
    }
   

    else if(board->player_board[user_x][user_y] == '0' && board->tiles[user_x][user_y] == '#'){ //if there is a 0, reveal until base cases are false
      board->tiles[user_x][user_y] = board->player_board[user_x][user_y];
      for(i = user_x - 1; i <= user_x + 1; i++){
	for(j = user_y -1; j <= user_y + 1; j++){
	  reveal(i, j, board);
	  
        
	  
	  
	}
      }
     
    }
  }
  
  return;
}

void place_numbers(Board* board){ //place numbers on the player board
  int i = 0;
  int j = 0;
  char bombs = '0';

  for(i = 0; i < board->rows; ++i){
    for(j = 0; j < board->cols; ++j){
      if(board->player_board[i][j] == '0'){
	if(i+1 < board->rows){
	  if(board->player_board[i+1][j] == '*'){
	    bombs += 1;
	  }
	}
	
	if(i-1 >= 0){
	  if(board->player_board[i-1][j] == '*'){
	    bombs += 1;
	  }
	}
	if(j+1 < board->cols){
	  if(board->player_board[i][j+1] == '*'){
	    bombs += 1;
	  }
	}
	if(j-1 >= 0){
	  if(board->player_board[i][j-1] == '*'){
	    bombs += 1;
	  }
	}
	if(i-1 >= 0 && j-1 >=0){
	  if(board->player_board[i-1][j-1] == '*'){
	    bombs += 1;
	  }
	}
	if(i-1 >= 0 && j+1 < board->cols){
	  if(board->player_board[i-1][j+1] == '*'){
	    bombs += 1;
	  }
	}
	if(i+1 < board->rows && j-1 >= 0){
	  if(board->player_board[i+1][j-1] == '*'){
	    bombs += 1;
	  }
	}
	if(i+1 < board->rows && j+1 < board->cols){
	  if(board->player_board[i+1][j+1] == '*'){
	    bombs += 1;
	  }
	}
	
	board->player_board[i][j] = bombs;
	bombs = '0';
      }
      else{
	continue;
      }
      
    }
  }
 
	


  return;
}



