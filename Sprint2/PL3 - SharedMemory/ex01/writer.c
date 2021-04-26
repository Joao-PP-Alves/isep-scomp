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
	
	shm_unlink ("/ex01");
	int fd = shm_open("/ex01", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);

    if (fd == -1) {
        printf("Error opening shared memory.\n");
        exit(EXIT_FAILURE);
    }
	
	ftruncate(fd, studentSize);
	studentInfo *student1 = mmap(NULL, studentSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	
    printf("Insert the student number:\n");
    scanf("%d", &student1->number);

    // Ask for input
    printf("Insert the student name:\n");
    scanf("%s", student1->name);
	
	printf("------------------------------------------------\n");
	printf("Shared Memory Student has the following info:\n");
	printf("Number: %d\n", student1->number);
	printf("Name: %s\n", student1->name);


    if (munmap((void *)student1, studentSize) < 0) {
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
