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

#define BUSY 1
#define FREE 0
#define WAY1 1
#define WAY2 2

typedef struct {
    int nrCarsWay1;
    int nrCarsWay2;   
	int bridgeBusy;
	int usedWay;
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
	//sem_unlink("/semMutex");
    //sem_unlink("/semWay1");
    //sem_unlink("/semWay2");
	//shm_unlink("/shm_ex16");
	
    sem_t *mutex;
    sem_t *semWay1Open;
    sem_t *semWay2Open;

    int fd;
	shmStruct *bridgeStruct;
	
	fd = shm_open("/shm_ex16", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, sizeof(shmStruct)) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	
	bridgeStruct = (shmStruct*)mmap(NULL, sizeof(shmStruct), PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if ((mutex = sem_open("/semMutex", O_CREAT)) == SEM_FAILED) {
        exit(1);
    }
    if ((semWay1Open = sem_open("semWay1", O_CREAT)) == SEM_FAILED) {
        exit(1);
    }
    if ((semWay2Open = sem_open("semWay2", O_CREAT)) == SEM_FAILED) {
        exit(1);
    }
	
    
    sem_wait(mutex);
    bridgeStruct->nrCarsWay2++;
    if (bridgeStruct->bridgeBusy == BUSY && bridgeStruct->usedWay != WAY2){
        sem_post(mutex);
        printf("Car going way 2 is waiting to cross bridge\n");
        sem_wait(semWay2Open);
    } else {
        if (bridgeStruct->bridgeBusy == FREE){
            bridgeStruct->bridgeBusy = BUSY;
            bridgeStruct->usedWay = WAY2;
        }
        sem_post(mutex);
    }
    printf("Car going way 2 is crossing bridge\n");
    sleep(5);
    sem_wait(mutex);
    bridgeStruct->nrCarsWay2--;
    
	if (bridgeStruct->nrCarsWay2 == 0){
        if (bridgeStruct->nrCarsWay1 != 0){
            bridgeStruct->usedWay = WAY1;
        } else {
            bridgeStruct->bridgeBusy = FREE;
            bridgeStruct->usedWay = FREE;
        }
        int i;
        for(i = 0; i < bridgeStruct->nrCarsWay1; i++){
            sem_post(semWay1Open);
        }
    }
    printf("Car going way 2 finished crossing bridge\n");
    sem_post(mutex);

	if(munmap(bridgeStruct, sizeof(shmStruct)) == -1) {
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	

    if (close(fd) == -1){
		perror("close fd failed.\n");
		exit(EXIT_FAILURE);
    }
/*	
	if (sem_close(mutex) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_close(semWay1Open) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_close(semWay2Open) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	
	if (sem_unlink("/semMutex") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/semWay1") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/semWay2") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }

	if (shm_unlink("/shm_ex16") < 0) {
        perror("Error at unlink shared memory");
        exit(EXIT_FAILURE);
    }
	*/
	exit(EXIT_SUCCESS);
}