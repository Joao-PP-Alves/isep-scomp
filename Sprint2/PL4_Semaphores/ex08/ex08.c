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

#define PROCESSES 2


int main(){
	sem_unlink("/ex09_S");
	sem_unlink("/ex09_C");
	
	sem_t *semS = sem_open("/ex09_S",O_CREAT|O_EXCL, 0644, 1);
	if(semS == SEM_FAILED){
		perror("Error creating the semaphore_1!\n");
		exit(EXIT_FAILURE);
    }
	sem_t *semC = sem_open("/ex09_C",O_CREAT|O_EXCL, 0644, 1);
	if(semC == SEM_FAILED){
		perror("Error creating the semaphore_2!\n");
		exit(EXIT_FAILURE);
    }
	
	pid_t pid = fork();
	if(pid == -1){
		perror("Error on fork system call.\n");
		exit(EXIT_FAILURE);
	}
	
	int i;
	if(pid == 0) {
		while (1) {
			sem_wait(semC);
			printf("C");
			fflush(stdout);
			sem_post(semS);
			sleep(1);		//just to not flood the screen
		}
		exit(EXIT_SUCCESS);
	}
	
	if (pid > 0) {
		while(1) {
			sem_wait(semS);
			printf("S");
			fflush(stdout);
			sem_post(semC);
			sleep(1);		//just to not flood the screen
		}
	}
	

	if (sem_close(semC) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_close(semS) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/ex09_S") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }    
	if (sem_unlink("/ex09_C") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }
	
	return 0;
}