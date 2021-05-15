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
#define ITERATIONS_TO_TEST 500

/*
Code lets 200 people in the train.
Constant ITERATIONS_TO_TEST defines the number of clients trying to get to the train throw each gate.
Only one client can enter or leave at a time.
Gate 1 is the parent process. Gate 2 and 3 the 2 sons.
*/
typedef struct{
	int passengers;
} train;

void enter_train(train* shared_data, int gate);
void leave_train(train* shared_data, int gate);

int main(){
	int j,i,interactions=0;
	int shm_field;
	int data_size = sizeof(train);
	sem_unlink("/ex11_sem");
	shm_unlink("/ex11_shm");
	shm_field = shm_open("/ex11_shm",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }

	train * shared_data = (train*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	sem_t *semaphore = sem_open("/ex11_sem",O_CREAT|O_EXCL, 0644, 1);
	srand(time(NULL));
	for(i = 0; i < CHILDS; i++){
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			while(interactions<ITERATIONS_TO_TEST){
				sem_wait(semaphore);
				int action = rand() % 5;
				if (action == 0){
					leave_train(shared_data,i+2);
				} else {
					enter_train(shared_data,i+2);
				}
				interactions++;
				sem_post(semaphore);
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
	while(interactions<ITERATIONS_TO_TEST){
		sem_wait(semaphore);
		int action = rand() % 5;
		if (action == 0){
			leave_train(shared_data,1);
		} else {
			enter_train(shared_data,1);
		}
		interactions++;
		sem_post(semaphore);
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

	if (shm_unlink("/ex11_shm") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }

	sem_unlink("/ex11_sem");
	shm_unlink("/ex11_shm");

	return 0;
}

void enter_train(train* shared_data,int gate){
	if (shared_data->passengers >= 200){
		printf("Train is full, unnable to enter! [%d] <Gate %d> \n",shared_data->passengers,gate);
	} else {
		printf("Entered Train! [%d] <Gate %d> \n",shared_data->passengers,gate);
		shared_data->passengers+=1;
	}
}

void leave_train(train* shared_data,int gate){
	shared_data->passengers-=1;
	printf("Left the train! [%d] <Gate %d> \n",shared_data->passengers,gate);
}
