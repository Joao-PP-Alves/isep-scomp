#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define TIME_TO_TASK 10
#define TIME_TO_SLEEP 20
#define BUFFER_STDIN 128
void handle_signal(int signo, siginfo_t *sinfo, void *context) {
	char buffer[43] = "\0";
	sprintf(buffer, "To slow, that is why the program will end!\n");
	write(STDOUT_FILENO, buffer, (sizeof(buffer)+1));
    kill(sinfo->si_pid, SIGKILL);
}

int main() {
    struct sigaction act;

    memset(&act,0,sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);

    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, &act, NULL);

    alarm(TIME_TO_TASK);

    char stdin_string[BUFFER_STDIN] = "\0";
    int string_size = 0;

    printf("Write a string\n");
    scanf("%s",stdin_string);

    string_size = strlen(stdin_string);
    printf("Size: %d\n", string_size);

    alarm(0); // Cancelled the previous alarm
    sleep(TIME_TO_SLEEP); 
    printf("Successfull execution!");
      
    return 0;
}