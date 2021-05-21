program: ex12 seller client 

ex12:  ex12.o
		gcc -Wall -g  ex12.o -o ex12 -lrt -lpthread

ex12.o: ex12.c
		gcc -Wall -g -c ex12.c -o ex12.o -lrt -lpthread
		
seller:  seller.o
		gcc -Wall -g  seller.o -o seller -lrt -lpthread

seller.o: seller.c
		gcc -Wall -g -c seller.c -o seller.o -lrt -lpthread

client:  client.o
		gcc -Wall -g  client.o -o client -lrt -lpthread

client.o: client.c
		gcc -Wall -g -c client.c -o client.o -lrt -lpthread

clean:
	rm *.o

run: ex12
	./ex12