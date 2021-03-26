#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int NUMBER_OF_CHILDREN = 6;

int spawn_childs(int n)
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
		
	int id = spawn_childs(NUMBER_OF_CHILDREN);

    if (id>0) {
        exit(id*2);
    }
	
    int i;
	for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
        int status;
        pid_t pid =wait(&status);
        if (WIFEXITED(status)) {
            printf("O filho com pid %d retornou: %d\n", pid, WEXITSTATUS(status));
        }
    }

	
	return 0;
}



