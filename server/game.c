#include <stdlib.h>
#include "game.h"

void initialize(struct data *playerData, unsigned char field[16][16]){

  unsigned char players = 2;
  unsigned char mines = 40;
  unsigned char i, n, t, y, x;

  
  unsigned char around[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, 
				{1, 0}, {1, -1}, {0, -1}, {-1, -1}};

  
  for(i = 0; i < players; i++){
    playerData[i].stats = 0;
    playerData[i].score = 0;
    playerData[i].toReceive = 0;
    playerData[i].toDiscard = 0;   
  }

  for(i = 0; i < 16; i++){
    for(n = 0; n < 16; n++){
      field[i][n] = 0;
    }
  }

  //set mines
  for(i = 0; i < mines; i++){
    y = (rand() % 16);
    x = (rand() % 16);
    while(field[y][x]){
      if(++x > 15){
	x = 0;
	if(++y > 15){
	  y = 0;
	}
      }
    }
    field[y][x] = 9;
  }
  
  //set numbers
  for(y = 0; y < 16; y++){
    for(x = 0; x < 16; x++){
      if(!field[y][x]){
	for(i = 0; i < 8; i++){
	  n = y + around[i][0];
	  t = x + around[i][1];
	  //n and t are unsigned char, so 0 - 1 > 16
	  if(n < 16 && t < 16){
	    field[y][x] += (field[n][t] == 9);
	  }
	}
      }
    }
  }

  //set empty mine fields
  for(i = 0; i < 16; i++){
    for(n = 0; n < 16; n++){
      field[i][n] |= 128;
    }
  }
}

int demine(unsigned char y, unsigned char x, unsigned char *mBuffer, unsigned char field[16][16]){
  unsigned char i, m, n, t;
  int l;

  
  unsigned char around[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, 
				{1, 0}, {1, -1}, {0, -1}, {-1, -1}};


  unsigned char lStack[255];

 if(!(field[y][x] & 128)){
    return 0;
  }


  //Reveal it
  //Push the first element
  lStack[0] = (y << 4) | x;
  l = 0;
  m = 0;
  field[y][x] &= 127;

  
  //Start popping
  while(l >= 0){
    x = lStack[l] & 15;
    y = (lStack[l] & 240) >> 4;
    mBuffer[m++] = lStack[l--];
    //If it's a zero, reveal more
    if(field[y][x]){
      continue;
    }

    //Reveal around if it's a zero
    for(i = 0; i < 8; i++){
      n = y + around[i][0];
      t = x + around[i][1];
      //n and t are unsigned char, so 0 - 1 > 16
      if(n < 16 && t < 16 && field[n][t] & 128){
	//If the square exists and is not revealed,
	//push it into the stack
	lStack[++l] = (n << 4) | t;
	//mark it
	field[n][t] &= 127;
      }
    }
  }

  return m;

}
