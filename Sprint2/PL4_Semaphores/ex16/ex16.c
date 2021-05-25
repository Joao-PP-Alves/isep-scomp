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
	for (i = 0; i < carsTotal; i++) {
		if (carsWay1 > 0) {
			fflush(0);
			execProgram("./carroSentido1");
			sleep(1);
			carsWay1--;
		}
		if (carsWay2 > 0) {
			fflush(0);
			execProgram("./carroSentido2");
			sleep(1);
			carsWay2--;
		}
	}



	for(i = 0; i < carsTotal; i++){
        wait(NULL);
    }
	printf("Bridge free for more cars!\n");

	

    return 0;
}