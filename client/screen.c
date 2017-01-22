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
#ifdef WIN32
    nodelay(stdscr, TRUE);
#endif

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
		    char *chatBuffer){}
