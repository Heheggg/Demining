#include <curses.h>
#include "defs.h"
#include "screen.h"

const char VALUE[11] = {' ', '1', '2', '3', '4', '5', '6', '7', '8', 'M', '#'};


int initializeScreen(WINDOW **left, WINDOW **middle, WINDOW **right,
    WINDOW **bottom, WINDOW **textBox){

    //Allocate the memory
    if(!(*left = newwin(17, 21, 0, 0)) ||
        !(*middle = newwin(17, COLS - 43, 0, 21)) ||
        !(*right = newwin(17, 21, 0, COLS - 22)) ||
        !(*bottom = newwin(LINES - 18, COLS, 17, 0)) ||
        !(*textBox = newwin(1, COLS, LINES-1, 0))){

        return 1;
    }

    //Enable scroll where we need it
    scrollok(*bottom, TRUE);
    //Enable arrow keys
    keypad(stdscr, TRUE);

    //Nodelay on windows because its select() sucks

    //Make pairs if colors are available
    if(has_colors()){
        start_color();

        init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_WHITE, COLOR_BLACK);
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(8, COLOR_RED, COLOR_BLACK);
        init_pair(9, COLOR_BLACK, COLOR_WHITE);
        init_pair(10, COLOR_WHITE, COLOR_BLACK);
        init_pair(11, COLOR_WHITE, COLOR_YELLOW);
        init_pair(12, COLOR_RED, COLOR_BLUE);
        init_pair(13, COLOR_RED, COLOR_GREEN);
        init_pair(14, COLOR_RED, COLOR_WHITE);
        init_pair(15, COLOR_RED, COLOR_YELLOW);
        init_pair(16, COLOR_RED, COLOR_CYAN);
        init_pair(17, COLOR_RED, COLOR_MAGENTA);
        init_pair(18, COLOR_WHITE, COLOR_CYAN);
    }

    return 0;
}

void drawMiddle(WINDOW *middle, unsigned char field[16][16]){
    unsigned int i, n;

    //Borders
    wborder(middle, ' ', ' ', ' ', '-', ' ', ' ', '-', '-');
    
    //The field stats completely unrevealed
    wcolor_set(middle, 11, NULL);

    for(i = 0; i < 16; i++){
        //Reset the cursor
        wmove(middle, i, ((COLS-42)/2)-8);
        for(n = 0; n < 16; n++){
            waddch(middle, '#');
        }
    }
}

void drawLeft(WINDOW *left, unsigned char players, unsigned char id, unsigned char mines, struct data *pData){

    unsigned int i;

    //Add borders
    wborder(left, ' ', '|', ' ', '-', ' ', '|', '-', '+');

    //Write all the information we have so far
    for(i = 0; i < players; i++){
        //Set the appropriate color
        wcolor_set(left, i + 2 , NULL);

        if(pData[i].stats){
            mvwprintw(left, 2*i, 0, "%s", pData[i].name);
            mvwprintw(left, 2*i, 17, "%3d", pData[i].score);

        }
        else {
            mvwprintw(left, 2*i, 0, "--------Open--------");
        }
    }

    //Print mines
    wcolor_set(left, 8, NULL);
    mvwprintw(left, 15, 0, "%3d/%3d mines", mines, mines);
}

void drawRight(WINDOW *right){
    //Borders
    wborder(right, '|', ' ', ' ', '-', '|', ' ', '+', '-');

    //Green
    wcolor_set(right, 3, NULL);

    mvwprintw(right, 11, 1, "Keys:");
    mvwprintw(right, 12, 1, "Arrows: Move Cursor");
    mvwprintw(right, 13, 1, "Space: Sweep");
    mvwprintw(right, 15, 1, "Return: Chat");
}

int keyboardHandler(WINDOW *middle, WINDOW *textBox,
    unsigned char *chatLen, unsigned char *isChatting, unsigned char *y, unsigned char *x,
    unsigned char id, unsigned char turn, unsigned char field[16][16],
		    char *chatBuffer){
  int c;
  c = getch();

  switch(c){
  case KEY_LEFT:
    if(turn != id || *isChatting){
      break;
    }

    setValue(middle, *y, *x, field[*y][*x] & 15, (field[*y][*x] & 240) >> 4);
    
    do{
      if(*x > 0){
	(*x)--;
      }else{
	*x = 15;
	if(*y >0){
	  (*y)--;
	}else{
	  *y = 15;
	}
      }
    }while(field[*y][*x] != 10);

    startCursor(middle, y, x, field);
    wrefresh(middle);
    break;

  case KEY_RIGHT:
    if(turn != id || *isChatting){
      break;
    }
    setValue(middle, *y, *x, field[*y][*x] & 15,
	     (field[*y][*x] & 240) >> 4);

    //Find an appropriate position
    do{
      if(*x < 15){
	(*x)++;
      }
      else {
	*x = 0;
	if(*y < 15){
	  (*y)++;
	}
	else {
	  *y = 0;
	}
      }
    }while(field[*y][*x] != 10);
    startCursor(middle, y, x, field);
    wrefresh(middle);
    break;

  case KEY_DOWN:
    if(turn != id || *isChatting){
      break;
    }

    //Fix the old spot
    setValue(middle, *y, *x, field[*y][*x] & 15,
	     (field[*y][*x] & 240) >> 4);

    //Find an appropriate position
    do{
      if(*y < 15){
	(*y)++;
      }
      else {
	*y = 0;
	if(*x < 15){
	  (*x)++;
	}
	else {
	  *x = 0;
	}
      }
    }while(field[*y][*x] != 10);
    startCursor(middle, y, x, field);
    wrefresh(middle);
    break;

  case KEY_UP:
    if(turn != id || *isChatting){
      break;
    }

    setValue(middle, *y, *x, field[*y][*x] & 15,
	     (field[*y][*x] & 240) >> 4);

    //Find an appropriate position
    do{
      if(*y > 0){
	(*y)--;
      }
      else {
	*y = 15;
	if(*x > 0){
	  (*x)--;
	}
	else {
	  *x = 15;
	}
      }
    }while(field[*y][*x] != 10);
    startCursor(middle, y, x, field);
    wrefresh(middle);
    break;

  case 10:
    if(*isChatting){
      //Send if we have stuff
      if(*chatLen > 2){
	return 1;
      }
      *isChatting = 0;
      mvwhline(textBox, 0, 0, ' ', COLS-18);
      *chatLen = 2;
    }else {
      *isChatting = 1;
      mvwprintw(textBox, 0, 0, "Say             : ");
    }
    wrefresh(textBox);
    break;

  case 127:
  case 8:
  case 263:
    if(*isChatting && *chatLen > 2){
      *chatLen -= 1;
      mvwaddch(textBox, 0, getcurx(textBox) - 1, ' ');
      wmove(textBox, 0, getcurx(textBox) - 1);
      wrefresh(textBox);
    }
    break;

  default:
    if(*isChatting && *chatLen < 64 && c > 31 && c <= 127){
      //Add it to the buffer
      chatBuffer[(*chatLen)++] = c;
      waddch(textBox, c);
      wrefresh(textBox);
    }else if(id == turn && !(*isChatting)){
      if(c == 32){
	return 2;
      }
    }
    break;
    
  }
  return 0;
}

void addLine(WINDOW *bottom, unsigned char msgLen, char *buffer,
    struct data *pData){

    unsigned char i;

    scroll(bottom);

    //Set the color
    wcolor_set(bottom, buffer[1] + 2, NULL);

    //Print name
    mvwprintw(bottom, LINES-19, 0,
        pData[(unsigned char) buffer[1]].name);
    wprintw(bottom, ": ");

    //Reset color
    wcolor_set(bottom, 0, NULL);
    //Add the zero and print the message
    buffer[msgLen] = 0;
    //Invalidate multi byte characters
    for(i = 0; buffer[i]; i++){
        if(buffer[i] < 32){
            buffer[i] = '?';
        }
    }
    //Print
    wprintw(bottom, buffer + 2 * sizeof(char));

    wrefresh(bottom);
}

void addPlayer(WINDOW *left, WINDOW *bottom, unsigned char id,
    struct data *pData){

    //Set the appropriate color
    wcolor_set(left, id + 2 , NULL);

    //Clean
    mvwhline(left, id * 2, 0, ' ', 20);

    //Print the name and score
    mvwprintw(left, 2 * id, 0, "%s", pData[id].name);
    mvwprintw(left, 2 * id, 17, "%3d", pData[id].score);

    //Tell the player
    wcolor_set(bottom, id + 2 , NULL);

    //Scroll and print the message
    scroll(bottom);
    mvwprintw(bottom, LINES-19, 0, "%s joined the game.", pData[id].name);

    wrefresh(left);
    wrefresh(bottom);
}

void removePlayer(WINDOW *left, WINDOW *bottom, unsigned char id,
    struct data *pData){

    //Remove the name
    mvwhline(left, id * 2, 0, ' ', 20);

    //Place Open
    wcolor_set(left, id + 2 , NULL);
    mvwprintw(left, 2*id, 0, "--------Open--------");

    //Tell the player
    wcolor_set(bottom, id + 2 , NULL);

    //Scroll and print the message
    scroll(bottom);
    mvwprintw(bottom, LINES-19, 0, "%s left the game.", pData[id].name);

    wrefresh(left);
    wrefresh(bottom);
}

void tellTurn(WINDOW *bottom, unsigned char turn, struct data *pData){
    //Set the color
    wcolor_set(bottom, turn + 2 , NULL);

    //Scroll and print the message
    scroll(bottom);
    mvwprintw(bottom, LINES-19, 0, "%s's turn.", pData[turn].name);

    wrefresh(bottom);
}

void setValue(WINDOW *middle, unsigned char y, unsigned char x,
    unsigned char v, unsigned char t){

    //Get the right color
    wcolor_set(middle, v != 9? v + 1 : t + 12, NULL);

    //Change the char
    mvwaddch(middle, y, (COLS-42)/2-8+x, VALUE[v]);

    //Move the cursor out of the way
    wmove(middle, 0, 0);

    wrefresh(middle);
}

void updateScore(WINDOW *left, unsigned char t, struct data *pData){
    //Set the appropriate color
    wcolor_set(left, t + 2 , NULL);

    //Print the name and score
    mvwprintw(left, 2 * t, 0, "%s", pData[t].name);
    mvwprintw(left, 2 * t, 17, "%3d", pData[t].score);

    wrefresh(left);
}


void startCursor(WINDOW *middle, unsigned char *y, unsigned char *x,
     unsigned char field[16][16]){

    //Set the appropriate color
    wcolor_set(middle, 18, NULL);
    //Find an unrevealed spot and move the cursor
    while((field[*y][*x] & 15) < 10){
      (*x)++;
      if(*x > 15){
	*x = 0;
	(*y)++;
	if(*y > 15){
	  *y = 0;
	}
      }
    }
    //Print an x
    mvwaddch(middle, *y, (COLS-42)/2-8+*x, 'x');
    

    //Move the cursor out of the way to avoid confusion
    wmove(middle, 0,0);

    wrefresh(middle);
}

 void updateMines(WINDOW *left, unsigned char mines){
   wcolor_set(left, 8, NULL);
   mvwprintw(left, 15, 0, "%3d", mines);

   wrefresh(left);
 }

void showWinner(WINDOW *bottom, unsigned char players, struct data *pData){
    unsigned char i, n, t;

    n = 0;
    t = 0;
    //Decide who's the winner
    for(i = 1; i < players; i++){
        if(pData[i].score > pData[n].score){
            n = i;
            t = 0;
        }
        else if(pData[i].score == pData[n].score){
            t++;
        }
    }

    //Print on the screen
    scroll(bottom);
    if(!t){
        //Set the color
        wcolor_set(bottom, n + 2, NULL);

        mvwprintw(bottom, LINES-19, 0, "The winner is: %s.", pData[n].name);
    }
    else {
        wcolor_set(bottom, 8, NULL);

        mvwprintw(bottom, LINES-19, 0, "Draw!");
    }
    scroll(bottom);

    wcolor_set(bottom, 0, NULL);
    mvwprintw(bottom, LINES-19, 0, "Press ENTER to continue.");

    wrefresh(bottom);


    //Wait for enter
    while(getch() != 10);
}
