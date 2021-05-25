program: ex16 carroSentido1 carroSentido2 

ex16:  ex16.o
		gcc -Wall -g  ex16.o -o ex16 -lrt -lpthread

ex16.o: ex16.c
		gcc -Wall -g -c ex16.c -o ex16.o -lrt -lpthread
		
carroSentido1:  carroSentido1.o
		gcc -Wall -g  carroSentido1.o -o carroSentido1 -lrt -lpthread

carroSentido1.o: carroSentido1.c
		gcc -Wall -g -c carroSentido1.c -o carroSentido1.o -lrt -lpthread

carroSentido2:  carroSentido2.o
		gcc -Wall -g  carroSentido2.o -o carroSentido2 -lrt -lpthread

carroSentido2.o: carroSentido2.c
		gcc -Wall -g -c carroSentido2.c -o carroSentido2.o -lrt -lpthread

clean:
	rm *.o

run: ex16
	./ex16