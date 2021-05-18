ex08: ex08.o 
	gcc -Wall -g ex08.o -o ex08 -lpthread

ex08.o: ex08.c 
	gcc -Wall -g -c ex08.c

run: ex08
	./ex08

clean:
	rm *.o ex08

debug:
	ddd ex08