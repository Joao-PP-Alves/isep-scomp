program: ex10 insert consult consult_all

ex10:  ex10.o
		gcc -Wall -g  ex10.o -o ex10 -lrt -lpthread

ex10.o: ex10.c
		gcc -Wall -g -c ex10.c -o ex10.o -lrt -lpthread
		
insert:  insert.o
		gcc -Wall -g  insert.o -o insert -lrt -lpthread

insert.o: insert.c
		gcc -Wall -g -c insert.c -o insert.o -lrt -lpthread

consult:  consult.o
		gcc -Wall -g  consult.o -o consult -lrt -lpthread

consult.o: consult.c
		gcc -Wall -g -c consult.c -o consult.o -lrt -lpthread

consult_all:  consult_all.o
		gcc -Wall -g  consult_all.o -o consult_all -lrt -lpthread

consult_all.o: consult_all.c
		gcc -Wall -g -c consult_all.c -o consult_all.o -lrt -lpthread

clean:
	rm *.o

run: ex10
	./ex10