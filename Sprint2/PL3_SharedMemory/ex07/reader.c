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

#define ARRAY_SIZE 10

int main(void) {
	int shMemSize = sizeof(ARRAY_SIZE) * sizeof(int); 
	int *arrayShMem;
	int fd = shm_open("/ex07", O_RDWR, S_IRUSR|S_IWUSR);
	
    if (fd == -1) {
        printf("Error opening shared memory. Please check writer.c!\n");
        exit(EXIT_FAILURE);
    }
	
	ftruncate(fd, shMemSize);
	arrayShMem = (int*) mmap(NULL, shMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	int array[ARRAY_SIZE];
	int i;
	for (i = 0; i < ARRAY_SIZE; i++) {
		array[i] = arrayShMem[i];
	}
	
	int sum = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		printf("Array[%d] = %d\n", i, array[i]);
		sum = sum + array[i];
	}
	
	printf("Sum = %d\n", sum);
	printf("Mean = %.2f\n", (double) sum/ARRAY_SIZE);

	
	
	if (munmap((void *)arrayShMem, shMemSize) < 0) {
        printf("Error unmapping at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
	
		

	
	printf("-------------- Reader exiting ----------------\n");		

	return 0;
	
}
