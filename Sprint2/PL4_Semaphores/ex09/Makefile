ex09: ex09.o 
	gcc -Wall -g ex09.o -o ex09 -lpthread

ex09.o: ex09.c 
	gcc -Wall -g -c ex09.c

run: ex09
	./ex09

clean:
	rm *.o ex09

debug:
	ddd ex09