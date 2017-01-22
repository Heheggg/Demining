#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include "networking.h"
#include "game.h"

int main(int argc, char *argv[]) {
  int players = 2;
  
  //Input buffer
  setlinebuf(stdout);
  setlinebuf(stderr);
  //Checks buffer
  if(argc != 2){
    fprintf(stderr, "Arguments: [Port]\n");
    fprintf(stderr, "Default: Port-%u\n", DEFAULTPORT);

    exit(1);
  }
  
  unsigned short port = DEFAULTPORT;
  
  if(!(port = strtoul(argv[1], NULL, 0))){
    fprintf(stderr, "Invalid port, must be {0-65535}.\n");
  }

  //setup server
  int server_Sock,*client_Sock;
  struct sockaddr_in server_Addr;
  struct sockaddr_in *client_Addr;
  struct data *playerData;
  
  switch(server_setup(&server_Sock, port, &server_Addr)){
    //Fallthrough
  case 1:
    fprintf(stderr, "The socket could not be created.\n");
  case 2:
    fprintf(stderr, "Unable to bind.\n");
  case 3:
    fprintf(stderr, "Unable to listen on port %u.\n", port);
    exit(4);
    //No error returned
  default:
    printf("Server listening on port %u.\n", port);
    printf("Waiting for players to connect.\n");
    break;
  }



  //seed
  srand(time(NULL));

  unsigned char field[16][16];
  
  while(1){
    
    if(!(client_Addr = (struct sockaddr_in *) malloc(players * sizeof(struct sockaddr_in))) ||
       !(playerData = (struct data *) malloc(players * sizeof(struct data))) ||
       !(client_Sock = (int *) malloc(players * sizeof(int)))){

      fprintf(stderr, "Not enough memory. \n");
      exit(5);
    }

    
    if(server_connect(&server_Sock, client_Sock, client_Addr, playerData)){
      fprintf(stderr, "Select failed.\n");
    }

    int pid = fork();
    if (pid < 0){
      fprintf(stderr,"ERROR on Fork \n");
    }

    if(pid == 0){
      close(server_Sock);

      
      free(client_Addr);
      free(playerData);
      free(client_Sock);
      
      exit(0);
    }else{
      int i;
      for(i = 0; i < players; i++){
	close(client_Sock[i]);
      }
    }
    
  
    return 0;
  
  }
}

