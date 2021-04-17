#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

const int ARRAY_SIZE = 1000;
int NUMBER_OF_CHILDREN = 5;
const int LEITURA = 0;
const int ESCRITA = 1;

int make_children(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}


int main(void) {
    int vec1[ARRAY_SIZE];
    int vec2[ARRAY_SIZE];
	
	srand(time(0));
	
	int i;
	for (i = 0; i < ARRAY_SIZE; i++) {
		vec1[i] = rand() % 10;
		vec2[i] = rand() % 10;
	}

    int fd[2];
    if(pipe(fd)==-1) { 
        perror("Pipe failed");
        return 1; 
    }

	int id = make_children(NUMBER_OF_CHILDREN);

    if (id == 0) {
        close(fd[ESCRITA]);
        int final = 0;
        int tmp = 0;
        for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
            int status;
            wait(&status);
            if (WIFEXITED(status)) {
                read(fd[LEITURA], &tmp, sizeof(tmp));
                final += tmp;
				printf("parent tmp is currently: %d\n", final);
            }
        }
        printf("Final sum is: %d\n", final);
    }
    if (id > 0) {
        close(fd[LEITURA]);
        int tmp = 0;
        int begin = id * 200;
        int end = begin + 200;
        int i;
        for (i = begin; i < end; i++) {
            tmp = tmp + vec1[i] + vec2[i];
        }
		printf("child tmp is currently: %d\n", tmp);

        write(fd[ESCRITA], &tmp, sizeof(tmp));
        close(fd[ESCRITA]);
    }
    return 0;
}
