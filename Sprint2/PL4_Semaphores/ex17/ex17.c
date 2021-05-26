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
#define TYPE_OF_SPECTATORS 3
#define MAX_SPECTATORS 300

typedef struct{
	int vip_queue;
    int special_queue;
    int normal_queue;
    int total_people;
} show_room;

int main(){
	int i,j=0,interactions=0;
	int shm_field;
	int data_size = sizeof(show_room);
	
	shm_unlink("/ex15_shm");
	shm_field = shm_open("/ex15_shm",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }

	show_room * shared_data = (show_room*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	sem_unlink("/ex15_sem1");
	sem_t *all_visitors = sem_open("/ex15_sem1",O_CREAT|O_EXCL, 0644, 300);
    shared_data->total_people=300;    
    shared_data->normal_queue=0;
    shared_data->special_queue=0;
    shared_data->vip_queue=0;
	srand(time(NULL));
	for(i = 0; i < TYPE_OF_SPECTATORS; i++){
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
            for (j=0;j<MAX_SPECTATORS/TYPE_OF_SPECTATORS;j++){       
            sem_wait(all_visitors);
            if(i==0)
                printf("VIP coming in.\n");
            if(i==1)
                printf("Special coming in.\n");
            if(i==2)
                printf("Normal coming in.\n");
            }
            int time_to_wait = rand() % 5 + 1;
            sleep(time_to_wait);
            //VIP
            if(i==0){
                for(;;){
                    shared_data->total_people--;
                    printf("VIP coming out.[%d]\n",shared_data->total_people);
                    sem_post(all_visitors);
                    sleep(10);
                    shared_data->vip_queue++;
                    printf("VIP in queue. [%d]\n",shared_data->vip_queue);
                    sem_wait(all_visitors);
                    shared_data->vip_queue--;
                    shared_data->total_people++;
                    printf("VIP coming in. [%d]\n", shared_data->total_people);
                    time_to_wait = rand() % 5 + 1;
                    sleep(time_to_wait);
                }
            }
            if (i==1){
                for(;;){
                    shared_data->total_people--;
                    printf("Special coming out. [%d]\n",shared_data->total_people);
                    sem_post(all_visitors);
                    sleep(6);
                    shared_data->special_queue++;
                    printf("Special in queue. [%d]\n",shared_data->special_queue);
                    while (shared_data->vip_queue!=0);
                    sem_wait(all_visitors);
                    shared_data->special_queue--;
                    shared_data->total_people++;
                    printf("Special coming in. [%d]\n",shared_data->total_people);
                    time_to_wait = rand() % 5 + 1;
                    sleep(time_to_wait);
                }
            }
            if (i==2){
                for(;;){
                    shared_data->total_people--;
                    printf("Normal coming out. [%d]\n",shared_data->total_people);
                    sem_post(all_visitors);
                    sleep(3);
                    shared_data->normal_queue++;
                    printf("Normal in queue. [%d]\n",shared_data->normal_queue);
                    while (shared_data->vip_queue!=0);
                    while (shared_data->special_queue!=0);
                    sem_wait(all_visitors);
                    shared_data->normal_queue--;
                    shared_data->total_people++;
                    printf("Normal coming in. [%d]\n",shared_data->total_people);
                    time_to_wait = rand() % 5 + 1;
                    sleep(time_to_wait);
                }
            }
            exit(EXIT_SUCCESS);
		}
	}
	
    int status;
	for(i = 0; i < TYPE_OF_SPECTATORS; i++){
		wait(&status);
	}

	if(munmap(shared_data, data_size) == -1){
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	

	if(close(shm_field) == -1){	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	if (shm_unlink("/ex15_shm") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }

	sem_unlink("/ex15_sem1");
	shm_unlink("/ex15_shm");

	return 0;
}