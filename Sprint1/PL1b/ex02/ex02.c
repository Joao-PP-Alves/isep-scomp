#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int main() {
    printf("pid : %d: ",getpid());
    for(;;){
    printf("I Love SCOMP!\n");
    sleep(1);
    }
}