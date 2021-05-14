#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>


typedef struct{
	int num1;
	int num2;
} numStrct;


#define REPEATS 1000000

int main(void) {

	shm_unlink ("/ex05");
	int fd = shm_open("/ex05", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
    if (fd == -1) {
        printf("Error opening shared memory. Please check writer.c!\n");
        exit(EXIT_FAILURE);
    }
	
	ftruncate(fd, sizeof(numStrct));
	numStrct *numeros = mmap(NULL, sizeof(numStrct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(numeros == NULL) {
		printf("Could not retreive info from shared memory! -Exiting-\n");
		return -1;
	}
	
	numeros->num1 = 8000;
	numeros->num2 = 200;
	
	
	pid_t pid = fork();
	int i, j = 0;
	
	
	if (pid == 0) {
		while (i < REPEATS) {
			numeros->num1--;
			numeros->num2++;
			i++;
		}
		exit(EXIT_SUCCESS);
	}
	if (pid > 0) {
		while (j < REPEATS) {
			numeros->num1++;
			numeros->num2--;
			j++;
		}	
	}
	
	wait(NULL);
	
	printf("num1: %d\n", numeros->num1);
    printf("num2: %d\n", numeros->num2);


    if (munmap((void *) numeros, sizeof(numStrct)) < 0) {
        printf("Error unmapping at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
	
	printf("-------------- Writer exiting ----------------\n");		

	return 0;
	
}

/*
Only write the value on the screen at the end. Review the results. Will these results always be correct?
-
No, the results will not be the same since both the parent and the child processes are incrementing "i".
So concurrently there's no way (as the program is now) to know how many times the parent or the child 
will increment/decrement the numbers in the structure.
The answer above was written in the case of the parent and child only were allowed to do their increment/decrement
for 1000000 times cumulatively

Since both of them have to increment/decrement 1000000 times my best guess is since both are trying to 
write on the same spaces of the shared memory, some of the increments/decrements are lost due to this 
concurrency


*/