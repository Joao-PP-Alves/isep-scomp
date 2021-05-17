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

#define MESSAGES 15

int main(){
	sem_unlink("/ex06_1");
	sem_unlink("/ex06_2");
	
	sem_t *semChild = sem_open("/ex06_1",O_CREAT|O_EXCL, 0644, 0); 
	if(semChild == SEM_FAILED){
		perror("Error creating the semaphore_1!\n");
		exit(EXIT_FAILURE);
    }
	sem_t *semParent = sem_open("/ex06_2",O_CREAT|O_EXCL, 0644, 0);
	if(semParent == SEM_FAILED){
		perror("Error creating the semaphore_2!\n");
		exit(EXIT_FAILURE);
    }

	sem_post(semChild); // initial value to 1

	
	pid_t pid = fork();
	if(pid == -1){
		perror("Error on fork system call.\n");
		exit(EXIT_FAILURE);
	}
	
	int i;
	if(pid == 0){
		for (i = 0; i < (MESSAGES / 2) + 1; i++) {
				sem_wait(semChild);
				printf("I'm the child!\n");
				sem_post(semParent);
		}
		exit(EXIT_SUCCESS);
	}
		
	if (pid > 0){
		for (i = 0; i < (MESSAGES / 2); i++) {
				sem_wait(semParent);
				printf("I'm the father!\n");
				sem_post(semChild);
		}
	}

	
	wait(NULL);

	if (sem_close(semParent) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_close(semChild) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/ex06_1") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }    
	if (sem_unlink("/ex06_2") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }

	return 0;
}