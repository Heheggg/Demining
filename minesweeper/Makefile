mine_sweeper.out: mine_sweeper.o main.o
	gcc -Wall -g -o mine_sweeper.out mine_sweeper.o main.o
	
main.o: main.c mine_sweeper.h
	gcc -Wall -g -c -o main.o main.c
	
mine_sweeper.o: mine_sweeper.c mine_sweeper.h
	gcc -Wall -g -c -o mine_sweeper.o mine_sweeper.c

run:
	./mine_sweeper.out
	
clean:
	rm *.out *.o
