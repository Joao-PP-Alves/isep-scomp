#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

int main(int argc, char * argv[]){
    printf("Process: %d\n", getpid());

	pid_t pidp;
    int i;
    struct timespec ts;
    
    //printf("pidp: %d!\n", pidp);
    printf("argv[1]: %s!\n", argv[1]);
    
    if(argc != 2){
        printf("Já foste!\n");
        exit(EXIT_FAILURE);
    }
    
    pidp = atoi(argv[1]);
    printf("pidp: %d!\n", pidp);
    
    
    kill(pidp, SIGUSR1);
    printf("Process %d Sent SIGUSR1 to %d!\n", getpid(), pidp);


}
