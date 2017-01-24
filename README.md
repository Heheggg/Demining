# Demining

Require: sudo apt-get install libncurses5-dev 


Compiling Guide:

	Server/Client folder:
		make - compile all the files

	      	make clean - remove all the compiled files

Running the game:

	Server: ./demining [port]
	
		Default: ./demining 1216
		
	Client: ./demining [ANYNAME] [IP] [Port]
	
		Default: ./demining sam 127.0.0.1 1216



How to play:

When the game starts, one of the two players will randomly start.

When a player goes, he has to keep selecting a tile until he finds one without a mine

The game will continue until all the mines are found, whoever landed on the least mines wins the game


To chat in the game, just press return/enter to submit your message. 
