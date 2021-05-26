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

#define WRITTERS 2
#define READERS 5

typedef struct {
    char string[100];	//PID and current time
    int currentReaders;
    int totalReaders;
    int totalWritters;
} shmStruct;


void execProgram(char *program) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(program, program, (char *)NULL);
        exit(EXIT_SUCCESS);
    }
    //while (wait(NULL) > 0);	// wait until children finishes execution
}

int main(int argc, char *agrv[]) {
    shm_unlink("/shm_ex14");
	sem_unlink("/sem_ex14_beingUsed");
	sem_unlink("/sem_ex14_barrier");
	sem_unlink("/sem_mutex");


    sem_t *semBeingUsed = sem_open("/sem_ex14_beingUsed", O_CREAT, 0644, 1);
	if (semBeingUsed == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
	sem_t *semBarrier = sem_open("/sem_ex14_barrier", O_CREAT, 0644, 1);
	if (semBarrier == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
	sem_t *mutex = sem_open("/sem_mutex", O_CREAT, 0644, 1);
	if (mutex == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }


    int fd;
	fd = shm_open("/shm_ex14", O_CREAT | O_RDWR , S_IRUSR|S_IWUSR);
    if (fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (fd, sizeof(shmStruct)) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }
	shmStruct * info = (shmStruct*) mmap(NULL, sizeof(shmStruct), PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(info == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	
	info->totalWritters = 0;
	info->totalReaders = 0;
	info->currentReaders = 0;
	
	//sprintf(info->string, "pid is %d, time is ..", getpid());

	
	
	int writters = WRITTERS;
	int readers = READERS;
	
	int i;
	
	for (i = 0; i < (WRITTERS + READERS); i++) {
		if (writters > 0) {
			execProgram("./writter");
			printf("writter created\n");
			writters--;
		}
		if (readers > 0) {
			execProgram("./reader");
			printf("reader created\n");
			readers--;
		}
	}
	

	for(i = 0; i < (WRITTERS + READERS); i++){
        wait(NULL);
    }
	
	printf("vai fechar\n");


	if(munmap(info, sizeof(shmStruct)) == -1) {
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	
	if(close(fd) == -1) {	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}
	if (shm_unlink("/shm_ex14") < 0) {
        perror("Unlink failed\n");
        exit(EXIT_FAILURE);
    }
	


	if (sem_close(semBeingUsed) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/sem_ex14_beingUsed") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }  

	if (sem_close(semBarrier) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/sem_ex14_barrier") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }  
	
	if (sem_close(mutex) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/sem_mutex") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }  



    return 0;
}