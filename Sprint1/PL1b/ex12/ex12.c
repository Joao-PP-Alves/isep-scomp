#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define NUMBER_OF_CHILDREN 50
#define VALUE_TO_FIND_SIMULA1 15

volatile sig_atomic_t success_counter = 0;
volatile sig_atomic_t failure_counter = 0;
time_t t;
void simula2() {
    printf("Process [%d] <-> simula2\n", getpid());
    int value = rand() % 30 + 1;

    printf("Process [%d] got number %d\n", getpid(), value);
    exit(0);
}

void simula1(int value) {
    printf("Process [%d] <-> simula1\n", getpid());

    printf("Process [%d] got number %d\n", getpid(), value);

    if (value == VALUE_TO_FIND_SIMULA1) {
        kill(getppid(), SIGUSR1);
    } else {
        kill(getppid(), SIGUSR2);
    }
    
    pause();
}

void handle_SIGUSR1(int signo, siginfo_t *sinfo, void *context) {
    success_counter++;
    printf("Success!! <-> S: %d | F: %d \n", success_counter, failure_counter);
}

void handle_SIGUSR2(int signo, siginfo_t *sinfo, void *context) {
    failure_counter++;
    printf("Failure!! <-> S: %d | F: %d \n", success_counter, failure_counter);
}

void handle_SIGUSR1_child(int signo, siginfo_t *sinfo, void *context) {
    simula2();
}

int main(int argc, char *argv[]){
    
    int i = 0, status=0;
    int children_pids[NUMBER_OF_CHILDREN];
    
    struct sigaction act, act2, child_act1;
    srand(time(&t));
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_SIGUSR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    memset(&act2, 0, sizeof(struct sigaction));
    sigemptyset(&act2.sa_mask);
    act2.sa_sigaction = handle_SIGUSR2;
    act2.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &act2, NULL);

    for (i = 0; i < NUMBER_OF_CHILDREN; i++){
        int value = rand() % 30 + 1;
        pid_t pid = fork();
        if (pid > 0){
            children_pids[i] = pid;
        } else {
            memset(&child_act1, 0, sizeof(struct sigaction));
            sigfillset(&child_act1.sa_mask);
            child_act1.sa_sigaction = handle_SIGUSR1_child;
            child_act1.sa_flags = SA_SIGINFO;
            sigaction(SIGUSR1, &child_act1, &act);
            simula1(value);
        }
    }

    while(success_counter+failure_counter < 25) {
        pause();
    }

    if (success_counter < 1) {
        printf("Inefficient algorithm!\n");
        for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
            kill(children_pids[i], SIGTERM); 
        }
        return 0;
    } else {
        printf("Good algorithm, starting formula 2 on remaining processes\n");
        for(i = 0; i < NUMBER_OF_CHILDREN; i++) {
            kill(children_pids[i], SIGUSR1); 
        }
    }

    for(i = 0; i < NUMBER_OF_CHILDREN; i++) {
        wait(&status);
    }

    return 0;

}