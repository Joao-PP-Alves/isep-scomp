#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define NUMBER_OF_SIGNALS 12
volatile sig_atomic_t SIGNAL_COUNTER = 1;

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
    sleep(2);
	char buffer[45] = "\0";
	sprintf(buffer, "SIGUSR1 signal captured, USR1_COUNTER = %d\n",SIGNAL_COUNTER++);
	write(STDOUT_FILENO, buffer, (sizeof(buffer)+1));
}

int main() {
    printf("Process started: %d\n", getpid());
    int signals[NUMBER_OF_SIGNALS] = {10,10,2,10,10,2,10,10,2,10,10,2};
    struct sigaction act;

    memset(&act,0,sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);

    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    pid_t pid = fork();
    if (pid == 0){
        struct timespec time, time2;
        time.tv_sec = 0;
        time.tv_nsec = 10000000;
        int i=0;

        for (i=0; i<NUMBER_OF_SIGNALS; i++){
            kill(getppid(),signals[i]);
            nanosleep(&time, &time2);
        }
        exit(0);
    }

     for(;;){
      printf("I'm working!\n");
      sleep(1);
     }
}