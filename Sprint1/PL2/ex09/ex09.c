#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define VEC_SIZE 50000
#define VEC_SIZE_PER_CHILD 5000
#define NUMBER_OF_CHILS 10
#define QAUNTITY 20

typedef struct{
    int customer_code;
    int product_code;
    int quantity;
} Sale ;

int main(){

    pid_t pids[NUMBER_OF_CHILS];
    Sale sales[VEC_SIZE];
    int i,j,status, pipe_field[2], products_aproved[VEC_SIZE], code_approved;

    srand((unsigned) time(NULL));
    for (i = 0; i < VEC_SIZE; i++) {
        Sale sale;
        sale.customer_code = rand() % 5000;
        sale.product_code = rand() % 5000;
        sale.quantity = rand() % 30;
        sales[i] = sale;
    }

    if (pipe(pipe_field) != 0){
        perror("Pipe failed!");
        exit(1);
    }

    for (i = 0; i < NUMBER_OF_CHILS; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
			close(pipe_field[0]);
			for (j = VEC_SIZE_PER_CHILD * i; j < (VEC_SIZE_PER_CHILD * (i+1)); j++) {
				if (sales[j].quantity > QAUNTITY) {
					write(pipe_field[1], &sales[j].product_code, sizeof(sales[j].product_code));
				}
			}
            close(pipe_field[1]);
			exit(0);
		}
    }
    close(pipe_field[1]);
    for (i=0; i<NUMBER_OF_CHILS; i++){
        waitpid(pids[i],&status,0);
    }
    i=0;
    while(read(pipe_field[0],&code_approved,sizeof(code_approved))){
        products_aproved[i] = code_approved;
        printf("Product aproved: %d\n", products_aproved[i++]);
    }
    close(pipe_field[0]);

    return 0;
}
