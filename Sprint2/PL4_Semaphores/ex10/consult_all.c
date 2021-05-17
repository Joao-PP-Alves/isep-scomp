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

typedef struct {
	int number;
	char name[NAME_SIZE];
	char address[ADDRESS_SIZE];
} personalRecord;

typedef struct {
	personalRecord entry[100];
	int index;
} sharedMemData;


int main(void){
	int fd = shm_open("/shm_ex10", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(0);
	}
	if(ftruncate(fd, sizeof(sharedMemData)) == -1) {
		perror("ftruncate ERROR\n");
		exit(0);
	}
	sharedMemData *dataBase = mmap(NULL, sizeof(sharedMemData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(dataBase == NULL) {
		perror("Error maping the object.\n");
		exit(0);
	}
	
	sem_t *sem;
	sem = sem_open("/sem_ex10_consult_all", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED){
		printf("Semaphore Error\n");
		return 0;
	}

	sem_wait(sem);

	if(dataBase->index == 0) {
		printf("No personal records available.\n");
	} else {
		int i;
		for(i = 0; i < dataBase->index; i++) {
			personalRecord *record = &(dataBase->entry[i]);
			printf("Personal Info: %d\nNumber: %d  Name: %s  Address: %s\n", i+1, record->number, record->name, record->address);
			printf("\n");
		}
	}
	printf("number of entries: %d\n", dataBase->index);

	sem_post(sem);

	sem_unlink("/sem_ex10_consult_all");

	munmap(dataBase, sizeof(sharedMemData));
	close(fd);
	printf("=========================================\n");

	return 0;
}
