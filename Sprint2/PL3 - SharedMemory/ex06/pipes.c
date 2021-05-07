#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>

#define VEC_SIZE 1000000
int main(void){
	int i, vec[VEC_SIZE];
	clock_t start_pipes_clock, end_pipes_clock;
	
	int pipe_field[2];
	
	if(pipe(pipe_field) == -1){ 
		perror("Pipe failed");
        exit(EXIT_FAILURE); 
    }
	
	srand((unsigned) time(NULL));

	for(i = 0; i < VEC_SIZE; i++){
		vec[i] = rand() % 10;
	}
		
	start_pipes_clock = clock();
	
	pid_t pid;

	if((pid = fork()) == -1){
		perror("Failed fork\n");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		
		int j;
		
		close(pipe_field[0]);
		
		for(j = 0; j < VEC_SIZE; j++){
			write(pipe_field[1], &vec[j], sizeof(vec[j]));
		}

		close(pipe_field[1]);
		exit(EXIT_SUCCESS);
	}
	close(pipe_field[1]);
	
	int values_from_pipe[VEC_SIZE];
	
	for(i = 0; i < VEC_SIZE; i++){
		read(pipe_field[0], &values_from_pipe[i], sizeof(values_from_pipe[i]));
	}
	close(pipe_field[0]);
    int status;
	wait(&status);
	
	end_pipes_clock = clock();
	
	float time_pipes;
    time_pipes = (float) (end_pipes_clock - start_pipes_clock) / CLOCKS_PER_SEC;

	printf("Time of pipes process: %f\n", time_pipes);

	return 0;
}