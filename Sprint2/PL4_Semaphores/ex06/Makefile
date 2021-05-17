ex06: ex06.o 
	gcc -Wall -g ex06.o -o ex06 -lpthread

ex06.o: ex06.c 
	gcc -Wall -g -c ex06.c

run: ex06
	./ex06

clean:
	rm *.o ex06

debug:
	ddd ex06