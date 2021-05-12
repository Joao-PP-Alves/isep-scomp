#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>

#define SIZE 10
#define TOTAL_DATA 30
typedef struct{
	int data_ready;
	int values[SIZE];
	int data_consumed;
} shared_memory_struct;

int main(void) {
	int shm_field, i = 0,k=0;
	int data_size = sizeof(shared_memory_struct);

	shm_unlink("/ex14");

	shm_field = shm_open("/ex14",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }
	shared_memory_struct * shared_data = (shared_memory_struct*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	
	/* Definir o valor a 100 */
	shared_data->data_ready = 0;
	shared_data->data_consumed = 1;
	srand(time(NULL)*getpid());
    pid_t p = fork();
    if (p == -1){
        perror("Fork Failed!");
        exit(EXIT_FAILURE);
    }
    if (p > 0){
        while(i<TOTAL_DATA){
			while(shared_data->data_consumed != 1);
			int j;
			for (j=0; j<SIZE; j++){
				shared_data->values[j] = rand() % 10;
				printf("Producer [%d] -> %d\n",j,shared_data->values[j]);
			}
			printf("========================\n");
			shared_data->data_ready=1;
			shared_data->data_consumed=0;
			i+=j;
		}
				
    } else {
		int aux[TOTAL_DATA];
        while(i<TOTAL_DATA){
			while(shared_data->data_ready != 1);
			int j;
			for (j=0; j<SIZE; j++){
				aux[k]= shared_data->values[j];
				printf("Consumer [%d] -> %d\n",k,aux[k]);
				k++;
			}
			printf("========================\n");
			shared_data->data_ready=0;
			shared_data->data_consumed=1;
			i+=j;
		}

		if(munmap(shared_data, data_size) == -1){
			perror("Munmap failed.\n");
			exit(EXIT_FAILURE);
		}	

		if(close(shm_field) == -1){
			perror("Cant close object.\n");
			exit(EXIT_FAILURE);
		}
        exit(EXIT_SUCCESS);
    }

	int status;
    wait(&status);

	if(munmap(shared_data, data_size) == -1){
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	

	if(close(shm_field) == -1){	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	if (shm_unlink("/ex14") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }

}