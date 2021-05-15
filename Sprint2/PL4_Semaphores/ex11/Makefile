ex11: ex11.o 
	gcc -Wall -g ex11.o -o ex11 -lpthread -lrt

ex11.o: ex11.c 
	gcc -Wall -g -c ex11.c

run: ex11
	./ex11

clean:
	rm *.o ex11

debug:
	ddd ex11