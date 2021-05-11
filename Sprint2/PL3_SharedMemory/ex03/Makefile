ex03: ex03.o
	gcc -g -Wall ex03.o -o ex03 -lrt

ex03.o: ex03.c
	gcc -g -Wall -c ex03.c -o ex03.o

reader: reader.o
	gcc -g -Wall reader.o -o reader -lrt

reader.o: reader.c
	gcc -g -Wall -c reader.c -o reader.o

clean:
	rm *.o

run: ex03 reader
	./ex03
	./reader
