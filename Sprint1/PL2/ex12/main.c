#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define NUMBER_OF_CHILDREN 5
#define LEITURA 0
#define ESCRITA 1
#define NUMBER_OF_PRODUCTS 5
#define COMM_PIPE 5

typedef struct {
	int id;
	int price;
	char name[20];
} product;

int make_children(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}


int main(void) {
	printf("Supermarket barcode readers!\n");
	
	product baseDados[NUMBER_OF_PRODUCTS];
	baseDados[0].id = 0;
	strcpy(baseDados[0].name, "Massa Espirais");
	baseDados[0].price = 11;
	baseDados[1].id = 1;
	strcpy(baseDados[1].name, "Esparguete");
	baseDados[1].price = 12;
	baseDados[2].id = 2;
	strcpy(baseDados[2].name, "Macarrão");
	baseDados[2].price = 13;
	baseDados[3].id = 3;
	strcpy(baseDados[3].name, "Aletria");
	baseDados[3].price = 14;
	baseDados[4].id = 4;
	strcpy(baseDados[4].name, "Cotovelos");
	baseDados[4].price = 15;


	int fd[NUMBER_OF_CHILDREN + 1][2];
	pipe(fd[COMM_PIPE]);
	pid_t arrayPids[5];	
	
	int i;
	for(i = 0; i < NUMBER_OF_CHILDREN; i++){
		pipe(fd[i]);
		arrayPids[i] = fork();

		if (arrayPids[i] == -1){
			printf("Error!\n");
			exit(1);
		}
		
		if(arrayPids[i] == 0){
			close(fd[COMM_PIPE][LEITURA]);
			
			int n;
			for (n = 0; n <= i; n++){
				close(fd[n][ESCRITA]);
			}
			
			int price, idProd = i;
			char name[20];
			printf("<- Scanner requesting product with id: %d \n", idProd);
			write(fd[COMM_PIPE][ESCRITA], &idProd, sizeof(idProd));
			
			read(fd[i][LEITURA], &name, sizeof(name));
			read(fd[i][LEITURA], &price, sizeof(price));
			printf("--- Product: %s - price: %d ---\n", name, price);
			return 0;
		}
	}
	
	close(fd[COMM_PIPE][ESCRITA]);
	int id, prodOut = 0;

	while(prodOut < NUMBER_OF_PRODUCTS){
		read(fd[COMM_PIPE][LEITURA], &id, sizeof(id));
		product p;
		
		int i;
		for(i = 0; i < NUMBER_OF_PRODUCTS; i++){
			if(baseDados[i].id == id){
				p.id = baseDados[i].id;
				p.price = baseDados[i].price;
				strcpy(p.name, baseDados[i].name);
			}
		}
		
		printf("-> DataBase sending info of product id: %d -> %s ;price %d\n", p.id, p.name, p.price);
		write(fd[id][ESCRITA], &p.name, sizeof(p.name));
		write(fd[id][ESCRITA], &p.price, sizeof(p.price));
		prodOut++;
	}

	for(i = 0; i < 5; i++) {
		wait(NULL);
	}

    return 0;
}
