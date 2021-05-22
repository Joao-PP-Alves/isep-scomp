ex15: ex15.o 
	gcc -Wall -g ex15.o -o ex15 -lpthread -lrt

ex15.o: ex15.c 
	gcc -Wall -g -c ex15.c

run: ex15
	./ex15

clean:
	rm *.o ex15

debug:
	ddd ex15