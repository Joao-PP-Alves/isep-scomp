#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <semaphore.h>

#define NUMBER_OF_CHILDREN 50

typedef struct {
    char str[50][80];
    int writen;
} shared_memory_struct;

int make_children(int n) {
    pid_t pid;
    int i;
	pid_t pidF = getpid();
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i; 	//returns childs with an incremented id
		else if (getpid() == pidF && i == n - 1)
			return pidF; // also returns father process after child making
    }
    return 0;
}


int main(void) {
	int shm_field;
	int data_size = sizeof(shared_memory_struct);

	shm_unlink("/shm_ex03");

	shm_field = shm_open("/shm_ex03", O_CREAT  | O_RDWR , S_IRUSR|S_IWUSR);
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }
	shared_memory_struct * shMemObj = (shared_memory_struct*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);
	if(shMemObj == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	
	sem_t *sem;
	sem = sem_open("/sem_ex03", O_CREAT, 0644, 1);
	if (sem == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

	int idFather = getpid();
	int id = make_children(NUMBER_OF_CHILDREN);	
	
    if (id >= 0 && id != idFather){
		sem_wait(sem);
        printf("Child writing - i:%d\n", id);
		sprintf(shMemObj->str[id], "I’m the Father - with PID %d", getpid());
		
		srand(time(NULL) * getpid());
		//sleep(rand() % 4 + 1);
		//sleep(1);
        sem_post(sem);

		if(munmap(shMemObj, data_size) == -1){
			perror("Munmap failed.\n");
			exit(EXIT_FAILURE);
		}	

		if(close(shm_field) == -1){	
			perror("Cant close object.\n");
			exit(EXIT_FAILURE);
		}

		exit(EXIT_SUCCESS);
    }
	
	// only father process
	int i;
	for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
		wait(NULL);
	}
	
	for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
		printf("%s - i:%d\n", shMemObj->str[i], i);
	}


	if(munmap(shMemObj, data_size) == -1) {
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	

	if(close(shm_field) == -1) {	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	if (shm_unlink("/shm_ex03") < 0) {
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }
	
    if (sem_close(sem) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("/sem_ex03") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }
	
	return 0;
}