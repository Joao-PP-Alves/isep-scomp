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

int main(){
	int j,i;
	sem_t *semaphore1 = sem_open("/ex09_1",O_CREAT|O_EXCL, 0644, 0);
	sem_t *semaphore2 = sem_open("/ex09_2",O_CREAT|O_EXCL, 0644, 0);

	for(i = 0; i < CHILDS; i++){
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			if (i==0){
				buy_chips();
				sem_post(semaphore1);
				sem_wait(semaphore2);
				eat_and_drink();
				exit(EXIT_SUCCESS);
			} else if(i==1) {
				buy_beer();
				sem_post(semaphore2);
				sem_wait(semaphore1);
				eat_and_drink();
				exit(EXIT_SUCCESS);
			}
		}
	}
    int status;
	for(i = 0; i < CHILDS; i++){
		wait(&status);
	}
	sem_unlink("/ex09_1");
	sem_unlink("/ex09_2");

	return 0;
}

void buy_chips(){
	printf("Got the chips!\n");
}

void buy_beer(){
	printf("Got the beer!\n");
}

void eat_and_drink(){
	printf("Eating and Drinking!\n");
}