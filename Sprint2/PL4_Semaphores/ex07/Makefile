ex07: ex07.o 
	gcc -Wall -g ex07.o -o ex07 -lpthread

ex07.o: ex07.c 
	gcc -Wall -g -c ex07.c

run: ex07
	./ex07

clean:
	rm *.o ex07

debug:
	ddd ex07