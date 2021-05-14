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
8 semaphores are used.
The print of the output will be converted to count how many numbers are in the file, instead of showing them.
Process syncronization successfull.
*/
int main(){
	int i, j;
	sem_t *semaphores[CHILDS];
    if (remove("output.txt") != 0){
        perror("Error deleting file output.txt.\n");
	}
	char name[7];
    for (i=1; i<=CHILDS; i++){
		sprintf(name, "/ex02_%d", i);
		sem_unlink(name);
		if (i==1)
			semaphores[i-1] = sem_open(name,O_CREAT|O_EXCL, 0644, 1);
		if (i>1)	
			semaphores[i-1] = sem_open(name,O_CREAT|O_EXCL, 0644, 0);
		if(semaphores[i-1] == SEM_FAILED){
			perror("Error creating the semaphore!\n");
			exit(EXIT_FAILURE);
		} 
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
			sem_wait(semaphores[i]);
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
			if (i!=CHILDS-1)
				sem_post(semaphores[i+1]);
			printf("Son %d will now kill himself successfully!\n",i+1);	
			exit(EXIT_SUCCESS);
		}
	}
    int status;
	for(i = 0; i < CHILDS; i++){
		wait(&status);
	}
	
	int num,contador=0;
	FILE * output;
	output = fopen("output.txt","r");
	if(output == NULL){
		perror("Error opening file.");
		exit(EXIT_FAILURE);
	}
	while(fscanf(output, "%d\n", &num) != EOF){
		contador++;
	}
	printf("Numbers in output file: %d\n", contador);
	printf("The father will now kill himself successfully!\n");

	
	fclose(output);
	for (i=1; i<=CHILDS; i++){
		sprintf(name, "/ex02_%d", i);
		sem_unlink(name);
	}
	return 0;
}