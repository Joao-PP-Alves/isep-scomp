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
	char buffer[68] = "\0";
	sprintf(buffer, "SIGUSR1 has been captured and was sent by the process with pid: %d\n", sinfo->si_pid);
	
	write(STDOUT_FILENO, buffer, (sizeof(buffer)+1));
}

int main (int argc, char *argv[]) {
    printf("Process started: %d\n", getpid());

    struct sigaction act;
    
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    
    for(;;){
    printf("I'm in a loop\n");
    sleep(4);
    }
    
    return 0;
}
	