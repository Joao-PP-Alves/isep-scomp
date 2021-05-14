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
#define LEITURA 0
#define ESCRITA 1

typedef struct{
	int data_ready;
	int values[SIZE];
	int data_consumed;
} shared_memory_struct;

int main(void) {
	int shm_field, i = 0,k=0;
	int data_size = sizeof(shared_memory_struct);
	int fdProducer[2];
	int fdConsumer[2];
	
	shm_unlink("/ex15");

	shm_field = shm_open("/ex15",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
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
	
	if (pipe(fdProducer) == -1) {
        perror("Pipe Producer failed");
		return 1;
    }
	if (pipe(fdConsumer) == -1) {
        perror("Pipe Consumer failed");
		return 1;
    }
	
	/* Definir o valor a 100 */
	int data_ready = 0;
	int data_consumed = 1;
	srand(time(NULL)*getpid());
    
	pid_t p = fork();
    if (p == -1){
        perror("Fork Failed!");
        exit(EXIT_FAILURE);
    }
    if (p > 0){
		close(fdProducer[LEITURA]);
		close(fdConsumer[ESCRITA]);
        while(i<TOTAL_DATA){
			
			int j;
			for (j=0; j<SIZE; j++){
				shared_data->values[j] = rand() % 10;
				printf("Producer [%d] -> %d\n",j,shared_data->values[j]);
			}
			printf("========================\n");
			
			data_ready = 1;
			data_consumed = 0;
			write(fdProducer[ESCRITA], &data_ready, sizeof(int));
						

			i+=j;
			read(fdConsumer[LEITURA], &data_consumed, sizeof(int));

		}
		close(fdProducer[ESCRITA]);
		close(fdConsumer[LEITURA]);
				
    } else {
		close(fdProducer[ESCRITA]);
		close(fdConsumer[LEITURA]);
		
		int aux[TOTAL_DATA];
        while(i<TOTAL_DATA){
			read(fdProducer[LEITURA], &data_ready, sizeof(int));

			int j;
			for (j=0; j<SIZE; j++){
				aux[k]= shared_data->values[j];
				printf("Consumer [%d] -> %d\n",k,aux[k]);
				k++;
			}
			printf("========================\n");
			
			data_ready = 0;
			data_consumed = 1;
			write(fdConsumer[ESCRITA], &data_consumed, sizeof(int));

			i+=j;

		}

		close(fdProducer[LEITURA]);
		close(fdConsumer[ESCRITA]);

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

	if (shm_unlink("/ex15") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }
	
	return 0;
}