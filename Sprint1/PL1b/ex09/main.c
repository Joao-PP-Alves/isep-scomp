#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

volatile sig_atomic_t taskAFinishSignaL = 0;

void handle_USR1 (int signo, siginfo_t *sinfo, void *context) {
	write(STDOUT_FILENO, "SIGUSR1 was received and handled.\n", 34);
	taskAFinishSignaL = signo;
}


int main() {
	printf("Father process: %d\n", getpid());
	
	pid_t procPid;
	if ((procPid = fork()) == -1) {
		printf("Fork failed\n");
		return -1;
	}

	int i;
	
	if (procPid > 0) {
		for (i = 1; i < 4; i++) {
			sleep(1);
			printf("Father process executing task A, %d seconds elapsed \n", i);
		}
        taskAFinishSignaL++;
		kill(procPid, SIGUSR1);
	} else if (procPid == 0) {
		
		struct sigaction act;
		memset(&act, 0, sizeof(struct sigaction));
		sigemptyset(&act.sa_mask);
		act.sa_sigaction = handle_USR1;
		act.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR1, &act, NULL);
		
		time_t t;
        srand((unsigned) time(&t));
		
		int duration = rand() % (4-0) + 1;
		sleep(duration);
		
		printf("Child process executing task B, %d seconds elapsed (random)\n", duration);
		
		
		while (taskAFinishSignaL == 0) {
            sleep(1);
        }
		
		printf("Child process executed task C after father process executed task A.\n");

	}
    return 0;
}
