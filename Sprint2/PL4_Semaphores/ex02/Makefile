ex02: ex02.o 
	gcc -Wall -g ex02.o -o ex02 -lpthread

ex02.o: ex02.c 
	gcc -Wall -g -c ex02.c

run: ex02
	./ex02

clean:
	rm *.o ex02

debug:
	ddd ex02