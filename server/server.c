#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );

int main(int argc, char *argv[]) {
  //Input buffer
  setlinebuf(stdout);
  setlinebuf(stderr);
  //Checks buffer
  if(argc != 2){
    fprintf(stderr, "Arguments: [Port]\n");
    fprint(stderr, "Default: Port-%u\n", DEFAULTPORT);

    exit(1);
  }
  
  unsigned short port = DEFAULTPORT;

  if(!(port = strtoul(argv[1], NULL, 0))){
    fprintf(stderr, "Invalid port, must be {0-65535}.\n")
  }

  
  int sd, connection;
  sd = server_setup();
    
  while (1) {

    connection = server_connect( sd );

    int f = fork();
    if ( f == 0 ) {

      close(sd);
      sub_server( connection );

      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}


void sub_server( int sd ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
  
}


void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

