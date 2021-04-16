#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define NUMBER_OF_CHILDREN 5
volatile sig_atomic_t counting_childs = 0;

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
    counting_childs--;
	char buffer[46] = "\0";
	sprintf(buffer, "\nChild just finished, I still have %d children\n",counting_childs);
	write(STDOUT_FILENO, buffer, (sizeof(buffer)+1));
}

int main () {
    struct sigaction act;
    int i=0,j=0,status=0;
    pid_t pids[NUMBER_OF_CHILDREN];

    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask);

    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO|SA_NOCLDWAIT|SA_NOCLDSTOP;
    sigaction(SIGCHLD, &act, NULL);

    for (i=0; i<NUMBER_OF_CHILDREN; i++)
    {
        pids[i] = fork();
        
        if (pids[i] > 0){
            counting_childs++;
        } else {
            for (j=i*200; j<=(i+5)*200; j++){
                printf("[%d]\n ",j);
            }

            exit(pids[i]);
        }
        
    }
    while (counting_childs != 0){
        pause();
        printf("paused\n");
    }

    for (j=0; j<NUMBER_OF_CHILDREN; j++){
        wait(&status);
    }

    return 0;
}