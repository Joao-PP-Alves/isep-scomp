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
#define VISITORS 10
#define VISITORS_PER_ROOM 5
#define SHOW_OPEN 1
#define SHOW_CLOSED 0
#define NUMBER_OF_SHOWS 3

typedef struct{
	int show;
} show_room;

int main(){
	int i,interactions=0;
	int shm_field;
	int data_size = sizeof(show_room);
	
	shm_unlink("/ex15_shm");
	shm_field = shm_open("/ex15_shm",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }

	show_room * shared_data = (show_room*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	sem_unlink("/ex15_sem1");
	sem_t *semaphore1 = sem_open("/ex15_sem1",O_CREAT|O_EXCL, 0644, 5);
	//sem_t *semaphore2 = sem_open("/ex15_sem2",O_CREAT|O_EXCL, 0644, 1);
	srand(time(NULL));
	shared_data->show=SHOW_CLOSED;
	for(i = 0; i < VISITORS; i++){
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			for(;;){
				sem_wait(semaphore1);
				if(shared_data->show == SHOW_OPEN){
					int time_to_wait = rand() % 5 + 1;
					printf("Visitor %d -> IN  for %d sec\n", i+1,time_to_wait);
					sleep(time_to_wait);
					printf("Visitor %d -> OUT\n", i+1);
				}
				sem_post(semaphore1);
			}
			exit(EXIT_SUCCESS);
		}
	}
	for(interactions=0;interactions<NUMBER_OF_SHOWS;interactions++){
		printf("Opened the show for 15 seconds\n");
		shared_data->show=SHOW_OPEN;
		sleep(15);
		shared_data->show=SHOW_CLOSED;
		printf("Closed the show for 10 seconds\n");
		sleep(10);
	}
	
    int status;
	for(i = 0; i < VISITORS; i++){
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

	if (shm_unlink("/ex15_shm") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }

	sem_unlink("/ex15_sem1");
	//sem_unlink("/ex15_sem2");
	shm_unlink("/ex15_shm");

	return 0;
}