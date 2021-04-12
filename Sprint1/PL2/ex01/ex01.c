#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>

int main(){
	
	pid_t p;
	int pid_pipe;
	int pipe_space[2]; // 0->leitura, 1->escrita
	if (pipe(pipe_space) == -1){
		perror("Pipe failed!");
		exit(1);
	}
	p = fork();
	
	if (p > 0){
		close(pipe_space[0]); //Fechar leitura (Não vai precisar)
		pid_pipe = getpid();
		printf("Parent pid (Writing): %d\n", pid_pipe);
		write(pipe_space[1], &pid_pipe, sizeof(int));
		close(pipe_space[1]); 
	}else{
		close(pipe_space[1]); //Fechar escrita (Não vai precisar)
		read(pipe_space[0], &pid_pipe, sizeof(int));
		printf("Parent pid (reading): %d\n", pid_pipe);
		close(pipe_space[0]);
	}
	
	return 0;
}