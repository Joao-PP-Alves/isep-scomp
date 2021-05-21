ex13: ex13.o 
	gcc -Wall -g ex13.o -o ex13 -lpthread -lrt

ex13.o: ex13.c 
	gcc -Wall -g -c ex13.c

run: ex13
	./ex13

clean:
	rm *.o ex13

debug:
	ddd ex13