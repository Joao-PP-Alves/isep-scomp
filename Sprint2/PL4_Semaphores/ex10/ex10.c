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

#define NAME_SIZE 30
#define ADDRESS_SIZE 100
#define MAX_RECORDS 100

typedef struct {
	int number;
	char name[NAME_SIZE];
	char address[ADDRESS_SIZE];
} personalRecord;

typedef struct {
	personalRecord entry[MAX_RECORDS];
	int index;
} sharedMemData;

int menu() {
    int choice;
    printf("1. Insert new personal info\n");
    printf("2. Consult a Record\n");
    printf("3. Consult all Records\n");
    printf("0. Exit\n");
    scanf("%d", &choice);
    return choice;
}

void execProgram(char *program) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(program, program, (char *)NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(NULL) > 0);	// wait until children finishes execution
}

int main(int argc, char *agrv[]) {
    shm_unlink("/shm_ex10");
	sem_unlink("/sem_ex10");


    sem_t *sem = sem_open("/sem_ex10", O_CREAT, 0644, 1);
	if (sem == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    int fd;
	fd = shm_open("/shm_ex10", O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (fd, sizeof(sharedMemData)) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }
	sharedMemData * dataBase = (sharedMemData*)mmap(NULL, sizeof(sharedMemData), PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(dataBase == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}

    sem_wait(sem);
    int i;
    for(i = 0; i < MAX_RECORDS; i++) {
        dataBase->entry[i].number = 0; // clean all dataBase fields info
    }
	dataBase->index = 0;
	
    sem_post(sem);

    int option;
    do {
        option = menu();

        switch (option) {
            case 1:
                execProgram("./insert");
                break;
            case 2:
                execProgram("./consult");
                break;
            case 3:
                execProgram("./consult_all");
                break;
            case 0:
                return EXIT_SUCCESS;
            default:
                option = menu();
        }
    } while(option != 0);


	if(munmap(dataBase, sizeof(sharedMemData)) == -1) {
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	
	if(close(fd) == -1) {	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}
	if (shm_unlink("/shm_ex10") < 0) {
        perror("Unlink failed\n");
        exit(EXIT_FAILURE);
    }

	if (sem_close(sem) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/sem_ex10") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }  


    return 0;
}

/*
Only now, discussing this exercise with a fellow colleague that I noticed that
this solution should involve several inserts and consults simultaneously... 
Obviously it's not working like that and I don't have the time to spend
to solve this issue.

*/