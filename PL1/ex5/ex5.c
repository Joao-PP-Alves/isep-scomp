#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int NR_OF_CHILDREN = 2;

int main(void) {
    pid_t pids[NR_OF_CHILDREN];

    int index;
    for (index = 0; index < NR_OF_CHILDREN; index++) {
        pids[index] = fork();

        if (pids[index] < 0) {
            perror("Fork failed!");
            exit(-1);
        }

        if (pids[index] == 0) {
            int time = index + 1;
            printf("%dº filho criado com o PID = %d (tempo de espera de %d segundos)\n", index+1, getpid(), time);
            sleep(time);
            exit(time);
        }
    }

    pid_t p;
    int status;
    for (index = 0; index < NR_OF_CHILDREN; index++) {
        p = waitpid(pids[index], &status, 0);
        if (WIFEXITED(status)) {
            printf("O %dº filho com o PID = %d terminou com valor = %d\n", index+1, p, WEXITSTATUS(status));
        }
    }

    return 0;
}