ex17: ex17.o 
	gcc -Wall -g ex17.o -o ex17 -lpthread -lrt

ex17.o: ex17.c 
	gcc -Wall -g -c ex17.c

run: ex17
	./ex17

clean:
	rm *.o ex17

debug:
	ddd ex17