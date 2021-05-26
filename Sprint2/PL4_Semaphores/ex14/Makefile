program: ex14 reader writter 

ex14:  ex14.o
		gcc -Wall -g  ex14.o -o ex14 -lrt -lpthread

ex14.o: ex14.c
		gcc -Wall -g -c ex14.c -o ex14.o -lrt -lpthread
		
reader:  reader.o
		gcc -Wall -g  reader.o -o reader -lrt -lpthread

reader.o: reader.c
		gcc -Wall -g -c reader.c -o reader.o -lrt -lpthread

writter:  writter.o
		gcc -Wall -g  writter.o -o writter -lrt -lpthread

writter.o: writter.c
		gcc -Wall -g -c writter.c -o writter.o -lrt -lpthread

clean:
	rm *.o

run: ex14
	./ex14