ex01: ex01.o 
	gcc -Wall -g ex01.o -o ex01 -lpthread

ex01.o: ex01.c 
	gcc -Wall -g -c ex01.c

run: ex01
	./ex01

clean:
	rm *.o ex01

debug:
	ddd ex01