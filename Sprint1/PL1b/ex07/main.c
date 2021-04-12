#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


void handle_USR1 (int signo, siginfo_t *sinfo, void *context) {
	const int SIGNAL_LIST = 64;
	sigset_t cmask;
	int signal;
	
	sigprocmask(0, NULL, &cmask);
	
	for (signal = 1; signal <= SIGNAL_LIST; signal++) {
        if(sigismember(&cmask, signal) == 1) {
            char buffer[38] = "\0";
            sprintf(buffer, "The Signal with number %d is blocked.\n", signal);
            write(STDOUT_FILENO, buffer, (sizeof(buffer)+1));
        }
    }
	
	write(STDOUT_FILENO, "SIGUSR1 was received and handled.\n", 34);
}


int main() {
	printf("Process: %d\n", getpid());

	struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
	sigfillset(&act.sa_mask); 
	act.sa_handler = handle_USR1;
	sigaction(SIGINT, &act, NULL);


	for(;;){
		printf("I Like Signals\n");
		sleep(2);
	}
	

    return 0;
}
	
	
	/*
	sigismember() returns 1 if signum is a member of set, 0 if signum is not a member, and -1 on error. 
	Which means that if a specific signal is not part of the member set, it will not be printed by
	the cycle present in the hanlde_USR1 function before main
	
	*/