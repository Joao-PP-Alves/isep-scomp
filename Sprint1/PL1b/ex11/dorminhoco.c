#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, char * argv[]) {
	if(argc != 2){
        printf("Illegal Argument Exception!\n");
        exit(EXIT_FAILURE);
    }

	int n = atoi(argv[1]);
	printf("Dorminhoco com pid: %d, vai dormir: %d segundos.\n", getpid(), n);
	sleep(n);
	printf("Dorminhoco: %d, acabou.\n", getpid());
    return 0;
}