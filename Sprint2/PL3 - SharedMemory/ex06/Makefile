shared_memory: shared_memory.o
	gcc -g -Wall shared_memory.o -o shared_memory -lrt
	
shared_memory.o: shared_memory.c
	gcc -g -Wall -c shared_memory.c -o shared_memory.o

pipes: pipes.o
	gcc -g -Wall pipes.o -o pipes -lrt
	
pipes.o: pipes.c
	gcc -g -Wall -c pipes.c -o pipes.o
	
clean:
	rm *.o

run: shared_memory pipes
	./shared_memory
	./pipes