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
#define CHILDS 3

int main(){
	int j,i;
	char name[7];
	sem_t *semaphores[CHILDS*2];
	for (i=1; i<=CHILDS*2; i++){
		sprintf(name, "/ex07_%d", i);
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
	
	for(i = 0; i < CHILDS; i++){
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			j=i+3;
			if (i==0){
				sem_wait(semaphores[i]);
				printf("Sistemas ");
			    fflush( stdout );
				sem_post(semaphores[i+1]);
				sem_wait(semaphores[j]);
				printf("a ");
			    fflush( stdout );
				sem_post(semaphores[j+1]);
				exit(EXIT_SUCCESS);
			} else if (i==1){
				sem_wait(semaphores[i]);
				printf("de ");
			    fflush( stdout );
				sem_post(semaphores[i+1]);
				sem_wait(semaphores[j]);
				printf("melhor ");
			    fflush( stdout );
				sem_post(semaphores[j+1]);
				exit(EXIT_SUCCESS);
			} else if (i==2){
				sem_wait(semaphores[i]);
			    printf("Computadores - ");
			    fflush( stdout );
				sem_post(semaphores[i+1]);
				sem_wait(semaphores[j]);
				printf("disciplina! \n");
			    fflush( stdout );
				exit(EXIT_SUCCESS);
			}
		}
	}
    int status;
	for(i = 0; i < CHILDS; i++){
		wait(&status);
	}
	for (i=1; i<=CHILDS*2; i++){
		sprintf(name, "/ex07_%d", i);
		sem_unlink(name);
	}
	printf("\n");
	fflush( stdout );
	return 0;
}