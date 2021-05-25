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


int main(void){
    sem_t *semClientsRequest = sem_open("/sem_ex12_clients_request", O_CREAT);
	if (semClientsRequest == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
	sem_t *semSellerFree = sem_open("/sem_ex12_seller_free", O_CREAT);
	if (semSellerFree == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
	sem_t *mutex = sem_open("/sem_mutex", O_CREAT);
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
	
	struct timespec waitingTime;
	clock_gettime(CLOCK_REALTIME, &waitingTime);
	waitingTime.tv_sec += 5;	//10 sec waiting max for next client


	while(sem_timedwait(semClientsRequest, &waitingTime) != -1) {
		clock_gettime(CLOCK_REALTIME, &waitingTime);
		waitingTime.tv_sec += 5;	//10 sec waiting max for next client
		
		sleep(queue->servicingTime);
		
		queue->ticketNumber = nextTicketNumber;
		printf("Seller sold ticket with number %d\n", nextTicketNumber);
		nextTicketNumber++;
		
		sem_post(semSellerFree);  
		//sem_post(semClientsRequest);
		
	}
	

	printf("Seller is closing the shop!\n");

	return 0;
}
