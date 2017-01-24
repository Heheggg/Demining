
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <curses.h>


#include <sys/socket.h>
#include <sys/unistd.h>

#include "defs.h"
#include "networking.h"
#include "screen.h"

int main(int argc, char *argv[]){
    //Check arguments
    if(argc < 2 || argc > 4){
        fprintf(stderr, "Usage: %s NAME [SERVERIP] [PORT]\n", argv[0]);
        fprintf(stderr, "By default SERVERIP=%s and PORT=%u.\n",
            DEFAULTIP, DEFAULTPORT);
        exit(1);
    }
    
    //Make sure we don't mess up the terminal
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);
    signal(SIGABRT, cleanup);

    unsigned short port = DEFAULTPORT;
    //p for current players
    unsigned char i, n, players, id, mines, p;
    int sock;
    struct data *pData = NULL;

    //Validate port
    if(argc == 4 && !(port = strtoul(argv[3], NULL, 0))){
        fprintf(stderr, "Invalid port.\n");
        exit(2);
    }

    //Check if it's a 7 bit name
    for(i = 0; argv[1][i]; i++){
        if(argv[1][i] < 32 || argv[1][i] > 127){
            fprintf(stderr,
                "Invalid name, only 7-bit characters are allowed.\n");
            exit(9);
        }
    }

    //Initialize curses
    initscr();
    noecho();

    //Check if it is big enough
    if(LINES < MINLINES || COLS < MINCOLS ){
        endwin();
        fprintf(stderr, "Your terminal has to be, at least, %ux%u.\n",
            MINCOLS, MINLINES);
        exit(3);
    }

    //Try to connect
    i = startSocket(&sock, argc >= 3? argv[2] : DEFAULTIP, port);
    if(i){
        endwin();
        switch(i){
            case 1:
                fprintf(stderr, "The socket could not be created.\n");
                exit(4);

            case 2:
                fprintf(stderr, "Unable to resolv %s.\n",
                    argc >= 3? argv[2] : DEFAULTIP);
                exit(5);

            default:
                fprintf(stderr, "Unable to connect to %s.\n",
                    argc >= 3? argv[2] : DEFAULTIP);
                exit(6);
        }
    }

    //Send name and retrieve basic info from the server
    p = exchangeBasics(sock, &players,&mines, &id,
        argv[1], &pData);

    //Lost connection
    if(!p){
        endwin();
        fprintf(stderr, "Lost connection to the server.\n");
        exit(7);
    }

    //Copy own name
    for(i = 0; argv[1][i] && i < 16; i++){
        pData[id].name[i] = argv[1][i];
    }
    pData[id].name[i] = 0;
    //Update own stats
    pData[id].stats = 1;

    //Initialize the field
    unsigned char field[16][16];

    for(i = 0; i < 16; i++){
        for(n = 0; n < 16; n++){
            field[i][n] = 10;
        }
    }

    //Draw the basic interface
    WINDOW *left, *middle, *right, *bottom, *textBox;

    if(initializeScreen(&left, &middle, &right, &bottom, &textBox)){
        fprintf(stderr, "Out of memory.\n");
        exit(8);
    }

    drawMiddle(middle, field);
    drawLeft(left, players, id, mines, pData);
    drawRight(right);

    refresh();
    wrefresh(left);
    wrefresh(middle);
    wrefresh(right);
    wrefresh(bottom);
    wrefresh(textBox);

    //Play
    p = game(left, bottom, textBox, middle, sock, id, players,
             mines, pData, field);

    shutdown(sock, 2);
    close(sock);
    if(!p){
        showWinner(bottom, players, pData);
    }

    //Close curses
    endwin();

    if(p){
        fprintf(stderr, "Lost connection to the server.\n");
    }
    
    //Close curses
    delwin(left);
    delwin(middle);
    delwin(right);
    delwin(bottom);
    delwin(textBox);

    //Free the memory
    free(pData);

    return 0;
}

//Close curses and the program
void cleanup(int param){
    endwin();

    exit(0);
}
