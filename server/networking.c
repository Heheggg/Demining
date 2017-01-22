#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include "game.h"
#include "networking.h"

int server_setup(int *server_Sock, unsigned short port, struct sockaddr_in *server_Addr){
  unsigned char i;
  int players = 2;

  //create socket
  *server_Sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(*server_Sock < 0){
    return 1;
  }

  //Sets up struct
  server_Addr -> sin_family = AF_INET;
  server_Addr -> sin_port = htons(port);
  server_Addr -> sin_addr.s_addr = htonl(INADDR_ANY);
  memset(server_Addr->sin_zero, 0, sizeof(server_Addr->sin_zero));

  //bind
  if(bind(*server_Sock,(struct sockaddr *) server_Addr,sizeof(struct sockaddr_in))){
    return 2;
  }

  return 0;
  
}

int server_connect(int *server_Sock, int *client_Sock, struct sockaddr_in *client_Addr, struct data *playerData){

  int players = 2;
  unsigned int size = sizeof(struct sockaddr_in);
  short int len;
  char buffer[191];
  unsigned char i;
  short int t;
  fd_set set;

  unsigned char p = 0;
  unsigned char n = 0;

  //Listen
  if(listen(*server_Sock,players)){
    return 3;
  }

  
  while(p < players){
    //Build playerse
    FD_ZERO(&set);
    for(i = 0; i < players; i++){
      if(playerData[i].stats){
	FD_SET(client_Sock[i],&set);
      }
    }
    FD_SET(*server_Sock, &set);

    if(!select(FD_SETSIZE, &set, NULL, NULL, NULL)){
      return 1;
    }

    if(FD_ISSET(*server_Sock, &set)){
      if(n < players){
	printf("New Connection. \n");
	t = -1;
	while(playerData[++t].stats & 1);
	client_Sock[t] = accept(*server_Sock,(struct sockaddr *)client_Addr, &size);
	playerData[t].stats |= 1;
	n++;
      }else{
	close(accept(*server_Sock,(struct sockAddr *) client_Addr, &size));
      }
    }

    for(i = 0; i < players; i++){
      //Skip if there's nothing on the socket
      if(!(FD_ISSET(client_Sock[i], &set))){
	continue;
      }

      //Receive if we can
      if(playerData[i].toReceive){
	len = recv(client_Sock[i],
		   playerData[i].buffer +
		   (playerData[i].len - playerData[i].toReceive) * sizeof(char),
		   playerData[i].toReceive, 0);

	if(len > 0){
	  playerData[i].toReceive -= len;
	  //Receive more if we didn't get the whole thing
	  if(playerData[i].toReceive){
	    continue;
	  }
	}
      }
      //Discard if we have to
      else if(playerData[i].toDiscard){
	len = recv(client_Sock[i], buffer, playerData[i].toDiscard, 0);
	if(len > 0){
	  playerData[i].toDiscard -= len;
	  continue;
	}
      }
      //Get the length of the new message
      else {
	len = recv(client_Sock[i], buffer, 1, 0);
	//Get the length if the player didn't leave
	if(len > 0){
	  //Adjust the values
	  if((unsigned char) buffer[0] > 64){
	    playerData[i].len = 64;
	    playerData[i].toReceive = 64;
	    playerData[i].toDiscard = (unsigned char) buffer[0] - 64;
	  }
	  else {
	    playerData[i].len = (unsigned char) buffer[0];
	    playerData[i].toReceive = (unsigned char) buffer[0];
	  }
	  continue;
	}
      }
      //Player left, remove it
      if(len <= 0){
	if(playerData[i].stats & 2){
	  printf("%s left.\n", playerData[i].name);
	  p--;
	}
	else {
	  printf("Unnamed left.\n");
	}
                
	//Reset Values
	playerData[i].toReceive = 0;
	playerData[i].toDiscard = 0;

	//Close the socket
	shutdown(client_Sock[i], 2);
	close(client_Sock[i]);
	
	//Tell the rest of the players
	buffer[0] = 2;
	buffer[1] = 'l';
	buffer[2] = i;
	for(t = 0; t < players; t++){
	  if(t != i && (playerData[t].stats & 3) == 3){
	    send(client_Sock[t], buffer, 3, 0);
	  }
	}
	//Remove player
	playerData[i].stats = 0;
	n--;
	continue;
      }

      //If we got down here, we're ready to parse
            
      //Read name or discard
      if(!(playerData[i].stats & 2)){
	//If not yet named
	//Copy name
	if(playerData[i].len > 16){
	  playerData[i].len = 16;
	}
	for(t = 0; t < playerData[i].len; t++){
	  playerData[i].name[t] = playerData[i].buffer[t];
	}
	playerData[i].name[playerData[i].len] = 0;
	//Update stats
	p++;
	playerData[i].namelen = playerData[i].len;
	playerData[i].stats |= 2;
	printf("Player %u's name is %s.\n", i, playerData[i].name);
	//Tell the other players
	sprintf(buffer, "0c0%s", playerData[i].name);
	buffer[0] = 2 + playerData[i].namelen;
	buffer[2] = i;
	for(t = 0; t < players; t++){
	  if(t != i && (playerData[t].stats & 3) == 3){
	    //Not the same, connected and named
	    send(client_Sock[t], buffer, playerData[i].namelen + 3, 0);
	  }
	}
	//Answer with all the info
	//Number of players, mines, bombs, player id, online players
	buffer[0] = 5;
	buffer[1] = players;
	buffer[2] = 40;
	buffer[3] = 0;
	buffer[4] = i;
	buffer[5] = p;
	send(client_Sock[i], buffer, 6, 0);
	//Send players info
	for(t = 0; t < players; t++){
	  if(t != i && (playerData[t].stats & 3) == 3){
	    sprintf(buffer, "00%s", playerData[t].name);
	    buffer[0] = playerData[t].namelen + 1;
	    buffer[1] = t;
	    send(client_Sock[i], buffer, playerData[t].namelen + 2, 0);
	  }
	}
      }
      //Player's chatting, retransmit
      else if(playerData[i].buffer[0] == 't'){
	//Make tBlabla into ltiBlabla
	//Where i = player id and l = length
	for(t = 1; t < playerData[i].len; t++){
	  buffer[2+t] = playerData[i].buffer[t];
	}
	buffer[0] = 1 + playerData[i].len;
	buffer[1] = 't';
	buffer[2] = i;
	for(t = 0; t < players; t++){
	  if(playerData[t].stats & 2){
	    send(client_Sock[t], buffer, playerData[i].len + 2, 0);
	  }
	}
	//Add a zero before printing
	playerData[i].buffer[playerData[i].len] = 0;
	printf("%s: %s\n", playerData[i].name,
	       playerData[i].buffer + sizeof(char));
      }
    }
  }
  return 0;

    
}

int play(int *client_Sock, struct data *playerData, unsigned char field[16][16]){
  unsigned char players = 2;
  
  unsigned char mines = 40;
  unsigned char turn = rand() % players;

  unsigned char i, n, t, e, swept;
  short int len;
  char buffer[191];
  unsigned char mBuffer[255];
  fd_set set;

  buffer[0] = 2;
  buffer[1] = 'v';
  buffer[2] = turn;

  for(i = 0; i < players; i++){
    send(client_Sock[i],buffer,3,0);
  }
  printf("%s turn.\n", playerData[turn].name);

  while(mines){
    FD_ZERO(&set);
    for(i = 0; i<players; i++){
      FD_SET(client_Sock[i], &set);
    }

    select(FD_SETSIZE, &set, NULL, NULL, NULL);

    //check each socket
    for(i = 0; i < players; i++){
      if(!(FD_ISSET(client_Sock[i], &set))){
	continue;
      }
      //Receive if we can
      if(playerData[i].toReceive){
	len = recv(client_Sock[i],
		   playerData[i].buffer +
		   (playerData[i].len - playerData[i].toReceive) * sizeof(char),
		   playerData[i].toReceive, 0);

	if(len > 0){
	  playerData[i].toReceive -= len;
	  //Receive more if we didn't get the whole thing
	  if(playerData[i].toReceive){
	    continue;
	  }
	}
      }
      //Discard if we have to
      else if(playerData[i].toDiscard){
	len = recv(client_Sock[i], buffer, playerData[i].toDiscard, 0);
	if(len > 0){
	  playerData[i].toDiscard -= len;
	  continue;
	}
      }
      //Get the length of the new message
      else {
	len = recv(client_Sock[i], buffer, 1, 0);
	//Get the length if the player didn't leave
	if(len > 0){
	  //Adjust the values
	  if((unsigned char) buffer[0] > 64){
	    playerData[i].len = 64;
	    playerData[i].toReceive = 64;
	    playerData[i].toDiscard = (unsigned char) buffer[0] - 64;
	  }
	  else {
	    playerData[i].len = (unsigned char) buffer[0];
	    playerData[i].toReceive = (unsigned char) buffer[0];
	  }
	  continue;
	}
      }
      //Player left
      if(len <= 0){
	printf("%s left.\n", playerData[i].name);
	//Close the socket
	shutdown(client_Sock[i], 2);
#ifdef WIN32
	closesocket(client_Sock[i]);
#else
	close(client_Sock[i]);
#endif
	//Tell the rest of the players
	buffer[0] = 2;
	buffer[1] = 'l';
	buffer[2] = i;
	for(n = 0; n < players; n++){
	  if(n != i){
	    send(client_Sock[n], buffer, 3, 0);
	  }
	}

	//End the game
	return 1;
      }

      //Parse
      if(playerData[i].buffer[0] == 't'){
	//Player's chatting, retransmit
	//Make tBlabla into ltiblabla\0
	//Where i = player id and l is the length
	for(n = 1; n < playerData[i].len; n++){
	  buffer[n + 2] = playerData[i].buffer[n];
	}
	buffer[0] = 1 + playerData[i].len;
	buffer[1] = 't';
	buffer[2] = i;
	for(n = 0; n < players; n++){
	  send(client_Sock[n], buffer, playerData[i].len + 2, 0);
	}
	//Add a zero before printing
	playerData[i].buffer[playerData[i].len] = 0;
	printf("%s: %s\n", playerData[i].name,
	       playerData[i].buffer + sizeof(char));
      }
      //Player swept
      else if(playerData[i].buffer[0] == 's' && i == turn){
	swept = demine((playerData[i].buffer[1] & 240) >> 4,
		      playerData[i].buffer[1] & 15, mBuffer, field);
	printf("%s swept %u, %u. Value: %u. %u cell(s).\n", 
	       playerData[i].name, (playerData[i].buffer[1] & 240) >> 4,
	       playerData[i].buffer[1] & 15,
	       field[(playerData[i].buffer[1]&240)>>4][playerData[i].buffer[1]&15],
	       swept);
	//If it was valid, tell the players
	if(swept){
	  sendReveal(swept, buffer, mBuffer, client_Sock, playerData,
		     players, field);
	  //Play again if it was a mine
	  if(field[(mBuffer[0] & 240) >> 4][mBuffer[0] & 15] != 9){
	    turn++;
	    turn %= players;
	  }
	  else{
	    playerData[i].score++;
	    mines--;
	    //Check if he won
	    for(t = 0; t < players; t++){
	      n = 0;
	      for(e = 0; e < players; e++){
		if(playerData[e].score + mines >= playerData[t].score){
		  n++;
		}
	      }
	      //End the game if he did
	      if(n == 1){
		//Tell other players and stop
		buffer[0] = 1;
		buffer[1] = 'w';
		for(e = 0; e < players; e++){
		  send(client_Sock[e], buffer, 2, 0);
		}
		return 0;
	      }
	    }
	  }
	  //Tell players who's next
	  buffer[0] = 2;
	  buffer[1] = 'v';
	  buffer[2] = turn;
	  for(n = 0; n < players; n++){
	    send(client_Sock[n], buffer, 3, 0);
	  }
	  printf("%s's turn.\n", playerData[turn].name);
	}
      }
    }
  }

  return 0;
}

void sendReveal(unsigned char swept, char *buffer,
		unsigned char *mBuffer, int *client_Sock, struct data *playerData,
		unsigned char players, unsigned char field[16][16]){

  unsigned char i, n;

  //Split and send
  buffer[1] = 'r';
  for(i = 0; i < swept;){
    //31 coordinates at a time
    buffer[0] = (swept - i) > 31? 63 : ((swept - i) * 2 + 1);

    for(n = 2; n < buffer[0]; n++){
      buffer[n++] = mBuffer[i];
      buffer[n] = field[(mBuffer[i] & 240) >> 4][mBuffer[i] & 15];
      i++;
    }
    for(n = 0; n < players; n++){
      send(client_Sock[n], buffer, buffer[0] + 1, 0);
    }
  }
}
