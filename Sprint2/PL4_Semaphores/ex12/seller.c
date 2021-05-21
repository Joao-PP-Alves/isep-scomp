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

#define NUMBER_OF_CLIENTS 2

typedef struct {
	int ticketNumber; 
	int servicingTime;  //sleep
} clientQueue;


int main(void){
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
	sem_t *mutex = sem_open("/sem_mutex", O_CREAT, 0644, 0);
	if (mutex == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    int fd;
	fd = shm_open("/shm_ex12", O_CREAT | O_RDWR , S_IRUSR|S_IWUSR);
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

		
	int nextTicketNumber = 0; 
	
	int i;
	for (i = 0; i < NUMBER_OF_CLIENTS; i++) {
		sem_wait(semClientsRequest); 
		sleep(queue->servicingTime);
		
		queue->ticketNumber = nextTicketNumber;
		printf("Seller sold ticket with number %d\n", nextTicketNumber);
		nextTicketNumber++;
		
		sem_post(semSellerFree);  
	} 
	
	for (i = 0; i < NUMBER_OF_CLIENTS; i++) {
		wait(NULL);
	}


	
	sem_unlink("/sem_ex12_clients_request");
	sem_unlink("/sem_ex12_seller_free");
	sem_unlink("/sem_mutex");

	munmap(queue, sizeof(clientQueue));
	close(fd);
	printf("Seller has no more clients\n");

	printf("================= Seller Exiting ========================\n");

	return 0;
}
