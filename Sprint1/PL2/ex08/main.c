#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define NUMBER_OF_CHILDREN 10
#define LEITURA 0
#define ESCRITA 1

typedef struct{
	char msg[20];
	int round;
} ticket;


int make_children(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}


int main(void) {
	printf("Win the pipe!\n");
	// if I remove the "\n" in the printf above the program bugs
	
	int fd[2];
	if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }
	
	int id = make_children(NUMBER_OF_CHILDREN);

	//int i = 0;
	if (id == 0) {
        close(fd[LEITURA]);
		int i = 0;
		
		for (i = 0; i < NUMBER_OF_CHILDREN; i++){
			ticket tryToWin;
			strcpy(tryToWin.msg, "Win");
			tryToWin.round = i + 1;
			write(fd[ESCRITA], &tryToWin, sizeof(tryToWin));
			
			sleep(2);
		}
		close(fd[ESCRITA]);
	}
	
	if (id > 0) {
		ticket tryToWin;
		close(fd[ESCRITA]);
        read(fd[LEITURA], &tryToWin, sizeof(tryToWin));
        close(fd[LEITURA]);
        printf(tryToWin.msg);
        printf(" round: %d (child: %d)\n", tryToWin.round, id);
        exit(tryToWin.round);
	}

	
	int i;
    int status;
    for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
        pid_t pid = wait(&status);
        if (WIFEXITED(status)) {
            int exit_round = WEXITSTATUS(status);
            printf("Child: %d won the round: %d\n", pid, exit_round);
        }
    }

    return 0;
}
