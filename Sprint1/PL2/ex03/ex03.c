#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

int main(){

	pid_t p;
	char hello_msg[] = "Hello World";
	char goodbye_msg[] = "Goodbye!";
	int pipe_field[2];
	pipe(pipe_field);
	p = fork();
	
	if (p < 0) {
        printf("Error on fork system call.");
		exit(1);
    }else if(p > 0){
        printf("My PID (parent) : %d\n", getpid());
        printf("Child's PID : %d\n",p);
		close(pipe_field[0]);		//Close Reading
		printf("1st message sent: %s\n", hello_msg);
		printf("2nd message sent: %s\n", goodbye_msg);
		write(pipe_field[1], hello_msg, sizeof(hello_msg));
		write(pipe_field[1], goodbye_msg, sizeof(goodbye_msg));
		close(pipe_field[1]);		//Close Writing
	}else{
		close(pipe_field[1]);		//Close Writing
		char hello_recieved[strlen(hello_msg) + 1];	//New string since hello_msg was filled before fork
		char goodbye_recieved[strlen(goodbye_msg) + 1];	//New string since goodbye_msg was filled before fork
	    read(pipe_field[0], hello_recieved, sizeof(hello_msg));
		read(pipe_field[0], goodbye_recieved, sizeof(goodbye_msg));
        printf("1st message recieved: %s\n", hello_recieved);
		printf("2nd message recieved: %s\n", goodbye_recieved);
		exit(close(pipe_field[0])); //Close Reading and return it's value
	}

    int status;
    pid_t pid =wait(&status);
    if (WIFEXITED(status)) {
         printf("O filho com pid %d retornou o valor %d\n", pid, WEXITSTATUS(status));
    }

	return 0;
}