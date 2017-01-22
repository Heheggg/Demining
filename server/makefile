CC=gcc
CFLAGS = -O2 -Wall

all: demining

clean:
	rm -f networking.o game.o main.o \

demining: networking.o server.o game.o
	$(CC) $(CFLAGS) -o demining  networking.o server.o game.o

server.o: server.c

networking.o: networking.c

game.o: game.c
