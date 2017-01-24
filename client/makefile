
CC=gcc
CFLAGS=-O2 -Wall

all: demining

clean:
	rm -f networking.o game.o client.o screen.o demining \

demining: networking.o client.o screen.o
	$(CC) $(CFLAGS)   -o demining client.o networking.o \
		screen.o -lncurses

windows: networking.o client.o screen.o
	$(CC) $(CFLAGS)   -o demining client.o networking.o \
		screen.o -lpdcurses -lws2_32

client.o: client.c 

networking.o: networking.c 

screen.o: screen.c 

game.o: game.c 
