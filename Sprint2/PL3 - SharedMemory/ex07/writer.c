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

void fill_array(int *array, int size){
    int i;
	srand(time(NULL) * getpid());
    for (i = 0; i < size; i++) {
		array[i] = (rand() % 20 + 1);
	}
}

int main(void) {
	int array[ARRAY_SIZE];
	int *arrayShMem;
	int shMemSize = sizeof(ARRAY_SIZE) * sizeof(int); 
	
	shm_unlink ("/ex07");
	int fd = shm_open("/ex07", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
    if (fd == -1) {
        printf("Error opening shared memory. Please check writer.c!\n");
        exit(EXIT_FAILURE);
    }
	
	ftruncate(fd, shMemSize);
	arrayShMem = (int*) mmap(NULL, shMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(arrayShMem == NULL) {
		printf("Could not retreive info from shared memory! -Exiting-\n");
		return -1;
	}
	
	fill_array(array, ARRAY_SIZE);

	int i;
	for (i = 0; i < ARRAY_SIZE; i++) {
		arrayShMem[i] = array[i];
	}

	
    if (munmap((void *)arrayShMem, shMemSize) < 0) {
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
