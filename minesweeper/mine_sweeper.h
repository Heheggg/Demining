#ifndef MINE_SWEEPER_H
#define MINE_SWEEPER_H


typedef struct board_struct{
  int rows;
  int cols;
  int mines;
  int seed;
  int argc;
  char** player_board;
  char** tiles;
  

 
}Board;

void read_args(Board* board, int argc, char** argv);
void create_board(Board* board);
void player_board(Board* board);
void destroy_board(Board* board);
void place_mines(Board* board);
void user_input(Board* board, int* user_x, int* user_y, int user_play);
void question(Board* board, int user_x, int user_y, int* user_play);
void marked(Board* board, int user_x, int user_y, int* user_play);
void menu_input(int* user_play, Board* board, int user_x, int user_y);
int menu_ops(Board* board, int user_play, int user_x, int user_y);
void print_board(Board* board);
void print_playboard(Board* board);
void place_numbers(Board* board);
void reveal(int user_x, int user_y, Board* board);
bool valid_play(int user_x, int user_y, Board* board);
void check_play(int user_x, int user_y, Board* board, int user_play);
void play_game(Board* board, int user_x, int user_y, int user_play);
bool game_won(Board* board);
void num_bombs_left(Board* board);



#endif
