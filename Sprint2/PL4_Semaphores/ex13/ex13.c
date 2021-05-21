#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <time.h>
#include <string.h>
#define CHILDS 2
#define ITERATIONS_TO_TEST 30
#define NUMBER_OF_VALUES 10

typedef struct{
	int numbers[NUMBER_OF_VALUES];
	int data_shared;
} shared_data_struct;

void enter_shared_data_struct(shared_data_struct* shared_data, int gate);
void leave_shared_data_struct(shared_data_struct* shared_data, int gate);

int main(){
	int i,interactions=0;
	int shm_field;
	int data_size = sizeof(shared_data_struct);
	
	shm_unlink("/ex13_shm");
	shm_field = shm_open("/ex13_shm",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }

	shared_data_struct * shared_data = (shared_data_struct*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	char name[7];
	sem_t *semaphores[CHILDS+1];
	for (i=1; i<=CHILDS+1; i++){
		sprintf(name, "/ex13_%d", i);
		sem_unlink(name);
		if (i==1)
			semaphores[i-1] = sem_open(name,O_CREAT|O_EXCL, 0644, 1);
		if (i>1)	
			semaphores[i-1] = sem_open(name,O_CREAT|O_EXCL, 0644, 0);
		if(semaphores[i-1] == SEM_FAILED){
			perror("Error creating the semaphore!\n");
			exit(EXIT_FAILURE);
		} 
	}
	shared_data->data_shared=0;
	srand(time(NULL));
	for(i = 0; i < CHILDS; i++){
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			while(interactions<ITERATIONS_TO_TEST/2){
				sem_wait(semaphores[0]);
				if (shared_data->data_shared < 10){
					int action = rand() % 5;
					shared_data->numbers[shared_data->data_shared] = action;
					printf("Producer%d [%d] -> %d\n", i+1, shared_data->data_shared, shared_data-> numbers[shared_data->data_shared]);
					shared_data->data_shared++;
					interactions++;
				}
				if (shared_data->data_shared == 10){
					sem_post(semaphores[1]);
					sem_wait(semaphores[2]);
				}
				sem_post(semaphores[0]);
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
	}
	int values[30];
	while(interactions<ITERATIONS_TO_TEST){
		sem_wait(semaphores[1]);
		for (i=0; i<NUMBER_OF_VALUES; i++){
			values[interactions] = shared_data->numbers[i];
			printf("Consumer [%d] -> %d\n", interactions, values[interactions]);
			interactions++;
		}
		shared_data->data_shared=0;
		sem_post(semaphores[2]);
	}
    int status;
	for(i = 0; i < CHILDS; i++){
		wait(&status);
	}

	if(munmap(shared_data, data_size) == -1){
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	

	if(close(shm_field) == -1){	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	if (shm_unlink("/ex13_shm") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }

	for (i=1; i<=CHILDS+1; i++){
		sprintf(name, "/ex13_%d", i);
		sem_unlink(name);
	}
	shm_unlink("/ex13_shm");

	return 0;
}