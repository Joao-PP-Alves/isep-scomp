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


void execProgram(char *program) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(program, program, (char *)NULL);
        exit(EXIT_SUCCESS);
    }
    //while (wait(NULL) > 0);	// wait until children finishes execution
}

int main(int argc, char *agrv[]) {

	int carsWay1 = 5;
	int carsWay2 = 3;
	int carsTotal = carsWay1 + carsWay2;
	

	int i;
	for (i = 0; i < carsWay1; i++) {
		printf("carroSentido1\n");
		fflush(0);
		execProgram("./carroSentido1");

	}
	
	for (i = 0; i < carsWay2; i++) {
		printf("carroSentido2\n");
		fflush(0);
		execProgram("./carroSentido2");
	}


	for(i = 0; i < carsTotal; i++){
        wait(NULL);
    }
	printf("Bridge closed!\n");

	

    return 0;
}