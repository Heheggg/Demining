CC=gcc
CFLAGS = -O2 -Wall

all: demining

clean:
	rm -f networking.o game.o main.o \

demining: networking.o server.o
	$(CC) $(CFLAGS) -o demining  networking.o server.o

server.o: server.c

networking.o: networking.c
