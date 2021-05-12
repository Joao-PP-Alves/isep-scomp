#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#define NUMBER_OF_CHANGES 1000000
#define INITIAL_VALUE 100
typedef struct {
	int value;
} shm_data_struct;

int main(void) {
	int shm_field, i = 0;
	int data_size = sizeof(shm_data_struct);

	shm_unlink("/ex08");

	shm_field = shm_open("/ex08",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }
	shm_data_struct * shared_data = (shm_data_struct*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	
	/* Definir o valor a 100 */
	shared_data->value = INITIAL_VALUE;

    pid_t p = fork();
    if (p == -1){
        perror("Fork Failed!");
        exit(EXIT_FAILURE);
    }


    if (p > 0){
        while (i < NUMBER_OF_CHANGES){
            shared_data->value += 1;
            shared_data->value -= 1;
            i++;
        }
    } else {
        while (i < NUMBER_OF_CHANGES){
            shared_data->value += 1;
            shared_data->value -= 1;
            i++;
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

	printf("Final Value: %d\n", shared_data->value);

	if(munmap(shared_data, data_size) == -1){
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	

	if(close(shm_field) == -1){
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	if (shm_unlink("/ex08") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }


	/**
	 * O Valor varia entre 90 a 110 (nos testes que fiz).
	 * É suposto?
	 */
}