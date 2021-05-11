#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>

#define VEC_SIZE 1000000
typedef struct{
	int values[VEC_SIZE];
} shm_data_struct;

int main(void){
	int data_size = sizeof(shm_data_struct), i, vec[VEC_SIZE];
	clock_t start_shm_clock, end_shm_clock;
	
	shm_unlink("/ex06");

	int pipe_field = shm_open("/ex06", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(pipe_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(pipe_field, data_size) == -1){
		perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
	}

	shm_data_struct* shared_data = (shm_data_struct*) mmap(NULL, data_size, PROT_READ |PROT_WRITE, MAP_SHARED, pipe_field, 0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	
	srand((unsigned) time(NULL));

	for(i = 0; i < VEC_SIZE; i++){
		vec[i] = rand() % 10;
	}
	
	start_shm_clock = clock();

	pid_t pid;

	if((pid = fork()) == -1){
		perror("Failed fork\n");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		
		int j;

		for(j = 0; j < VEC_SIZE; j++){  
			shared_data->values[j] = vec[j];
		}

		exit(EXIT_SUCCESS);
	}

    int status;
	wait(&status);

    int values_from_shm[VEC_SIZE];
    for(i = 0; i < VEC_SIZE; i++){
		values_from_shm[i] = shared_data->values[i];
	}
    

	end_shm_clock = clock();

	float time_shm = (float) (end_shm_clock - start_shm_clock) / CLOCKS_PER_SEC;

	printf("Time of shared memory process: %f\n", time_shm);

	if(munmap(shared_data, data_size) == -1){
		perror("Cant read object.\n");
		exit(EXIT_FAILURE);
	}

	if(close(pipe_field) == -1){
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}