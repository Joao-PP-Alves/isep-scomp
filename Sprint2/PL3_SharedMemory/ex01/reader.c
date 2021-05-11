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

typedef struct{
	int number;
	char name[100];
}studentInfo;


int main(void) {
	int studentSize = sizeof(studentInfo);
	
	int fd = shm_open("/ex01", O_RDWR, S_IRUSR|S_IWUSR);
	
    if (fd == -1) {
        printf("Error opening shared memory. Please check writer.c!\n");
        exit(EXIT_FAILURE);
    }
	
	ftruncate(fd, studentSize);
	studentInfo *student1Again = mmap(NULL, studentSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(student1Again == NULL) {
		printf("Could not retreive info from shared memory! -Exiting-\n");
		return -1;
	}
	
	printf("------------------------------------------------\n");		
	printf("Reader accessed shared memory Student and it has the following info:\n");
	printf("Number: %d\n", student1Again->number);
	printf("Name: %s\n", student1Again->name);
	
	
	if (munmap((void *)student1Again, studentSize) < 0) {
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
