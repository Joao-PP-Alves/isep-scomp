#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

const int ARRAY_SIZE = 2000;
int NUMBER_OF_CHILDREN = 10;

int make_children(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}


int main() {
	
	int randoms [ARRAY_SIZE];
	int n;
	
	time_t t; //for srand
	srand((unsigned) time(&t));
	
	int i;
	for (i = 0; i < ARRAY_SIZE; i++) {
		randoms[i] = rand() % 100;
	}
	
/*	for (i = 0; i < ARRAY_SIZE; i++) {
		printf("%d ", randoms[i]);
	}
*/	
    n = rand() % 100;
	int id = make_children(NUMBER_OF_CHILDREN);
	
	if (id > 0) {
        int start = id * 100;
        int finish = (id + 2) * 100;
        for (i = start; i < finish; i++) {
            if (randoms[i] == n) {
                exit(i - start);
			}
        }
        exit(255);
    }
	
	for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
        int status;
        pid_t pid =wait(&status);
        if (WIFEXITED(status)) {
            printf("O filho com pid %d retornou: %d\n", pid, WEXITSTATUS(status));
        }
    }

	
	return 0;
}



