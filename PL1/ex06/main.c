#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int NR_OF_PROCESSES = 4;

int main() {
	
    int i = 0;
    pid_t pids[NR_OF_PROCESSES];
	int count = 0;
    printf("Este é o inicio do processo pai: %d.\n", getpid());
	
    for (i = 0; i < NR_OF_PROCESSES; i++) {
		pid_t p = fork();
		if (p == 0) {
			//sleep(1);
			printf("Este é o inicio e fim do processo: %d.\n", getpid());
			exit(0);
		}
		if (p % 2 == 0){
			pids[count] = p;
			count++;
		}
    }
	
	int status;	
    for (i = 0; i < count; i++) {
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status)) {
			//printf("O filho com o PID = %d terminou com valor = %d\n", index+1, getpid(), WEXITSTATUS(status));
		}
	}
  
	
    printf("Este é o fim do processo: %d.\n", getpid());
	printf("This is the end.\n");
	return 0;
}



