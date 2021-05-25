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

#define NUMBER_OF_CLIENTS 10

typedef struct {
	int ticketNumber; 
	int servicingTime;  //sleep
} clientQueue;

typedef struct {
	int nextTicketNumber;
} ticketSale;



void execProgram(char *program) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(program, program, (char *)NULL);
        exit(EXIT_SUCCESS);
    }
    //while (wait(NULL) > 0);	// wait until children finishes execution
}

int main(int argc, char *agrv[]) {
    shm_unlink("/shm_ex12");
	sem_unlink("/sem_ex12_clients_request");
	sem_unlink("/sem_ex12_seller_free");
	sem_unlink("/sem_mutex");


    sem_t *semClientsRequest = sem_open("/sem_ex12_clients_request", O_CREAT, 0644, 0);
	if (semClientsRequest == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
	sem_t *semSellerFree = sem_open("/sem_ex12_seller_free", O_CREAT, 0644, 1);
	if (semSellerFree == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
	sem_t *mutex = sem_open("/sem_mutex", O_CREAT, 0644, 1);
	if (mutex == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }


    int fd;
	fd = shm_open("/shm_ex12", O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (fd, sizeof(clientQueue)) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }
	clientQueue * queue = (clientQueue*)mmap(NULL, sizeof(clientQueue), PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(queue == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}

	//sem_wait(semClientsRequest); // blocks any client to request a ticket before seller is executed.

 
	int sellers = 1;
	int buyers = NUMBER_OF_CLIENTS;
	
	int i;
	for (i = 0; i < NUMBER_OF_CLIENTS; i++) {
		if (sellers > 0) {
			execProgram("./seller");
			sellers--;
		}
		if(buyers > 0) {
			execProgram("./client");
			buyers--;
		}
	}
	
	for(i = 0; i < NUMBER_OF_CLIENTS + 1 ; i++){
        wait(NULL);
    }
	
	printf("vai fechar.\n");


	if(munmap(queue, sizeof(clientQueue)) == -1) {
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	
	if(close(fd) == -1) {	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}
	if (shm_unlink("/shm_ex12") < 0) {
        perror("Unlink failed\n");
        exit(EXIT_FAILURE);
    }
	


	if (sem_close(semClientsRequest) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/sem_ex12_clients_request") < 0) {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }  

	if (sem_close(semSellerFree) < 0) {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
	if (sem_unlink("/sem_ex12_seller_free") < 0) {
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