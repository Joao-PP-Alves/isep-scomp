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


int main(){
	sem_unlink("/ex05");
	sem_t *semaphore = sem_open("/ex05",O_CREAT|O_EXCL, 0644, 0);
	if(semaphore == SEM_FAILED){
		perror("Error creating the semaphore!\n");
		exit(EXIT_FAILURE);
    }
	
	pid_t pid = fork();
	if(pid == -1){
		perror("Error on fork system call.\n");
		exit(EXIT_FAILURE);
	}
	if(pid == 0){
		printf("I'm the child!\n");
		sem_post(semaphore);
		exit(EXIT_SUCCESS);
	}
	sem_wait(semaphore);
	printf("I'm the father!\n");
	wait(NULL);
	sem_unlink("/ex05");
	return 0;
}