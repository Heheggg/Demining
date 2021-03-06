#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 1000
#define DEFAULTIP 127.0.0.1
#define DEFAULTPORT 1216


void error_check( int i, char *s );

int server_setup(int *server_Sock, unsigned short port, struct sockaddr_in *server_Addr);
int server_connect(int *server_Sock, int *client_Sock, struct sockaddr_in *client_Addr, struct data *playerData);

int play(int *client_Sock, struct data *playerData, unsigned char field[16][16]);

void sendReveal(unsigned char swept, char *buffer,
		unsigned char *mBuffer, int *client_Sock, struct data *playerData,
		unsigned char players, unsigned char field[16][16]);

int client_connect( char *host );

#endif

