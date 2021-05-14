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
#define CHILDS 8
#define EACH_CHILD_LIMIT 200

/*
The file 200Numbers.txt has already 200 numbers in it.
The child processes will all read those 200 numbers (from 1 to 200).
The output file should have 1600 numbers (8* the numbers in the previous file).
*/
int main(){
	int i, j;
    sem_unlink("/ex01");

    if (remove("output.txt") != 0){
        perror("Error deleting file output.txt.\n");
        exit(EXIT_FAILURE);
    }
    
	sem_t *semaphore = sem_open("/ex01",O_CREAT|O_EXCL, 0644, 1);
	if(semaphore == SEM_FAILED){
		perror("Error creating the semaphore!\n");
		exit(EXIT_FAILURE);
	} 
	
	FILE * file_to_read;
	FILE * file_to_write;
	
	for(i = 0; i < CHILDS; i++){
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			sem_wait(semaphore);
			file_to_read = fopen("200Numbers.txt", "r");
			file_to_write = fopen("output.txt", "a"); 
			
            int k=0;
            int num[EACH_CHILD_LIMIT];
            for (k=0;k<EACH_CHILD_LIMIT;k++){
                fscanf(file_to_read,"%d",&num[k]);
                fprintf(file_to_write,"%d\n",num[k]);
            }
			
			fclose(file_to_read);
			fclose(file_to_write);
			sem_post(semaphore);
			exit(EXIT_SUCCESS);
		}
	}
    int status;
	for(i = 0; i < CHILDS; i++){
		wait(&status);
	}
	
	int num;
	FILE * output;
	output = fopen("output.txt","r");
	if(output == NULL){
		perror("Error opening file.");
		exit(EXIT_FAILURE);
	}
	while(fscanf(output, "%d\n", &num) != EOF){
		printf("%d\n", num);
	}
	
	fclose(output);
	sem_unlink("/ex01");
	
	return 0;
}