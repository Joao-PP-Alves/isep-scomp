#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


void handle_INT (int signo, siginfo_t *sinfo, void *context) {
	char buffer[40] = "\0";
	sprintf(buffer, "I won’t let the process end with CTRL-C!\n");
	
	write(STDOUT_FILENO, buffer, (sizeof(buffer)+1));
}

void handle_QUIT (int signo, siginfo_t *sinfo, void *context) {
	char buffer2[48] = "\0";
	sprintf(buffer2, "I won’t let the process end by pressing CTRL-\\! \n");
	
	write(STDOUT_FILENO, buffer2, (sizeof(buffer2)+1));
}

int main() {
	printf("Process: %d\n", getpid());

	struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_INT;
    act.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &act, NULL);


	struct sigaction act2;
    memset(&act2, 0, sizeof(struct sigaction));
    sigemptyset(&act2.sa_mask);
    act2.sa_sigaction = handle_QUIT;
    act2.sa_flags = SA_SIGINFO;
    sigaction(SIGQUIT, &act2, NULL);
	
	
	
	for(;;){
		printf("I Like Signal\n");
		sleep(1);
	}
	

    
	
    return 0;
}
	