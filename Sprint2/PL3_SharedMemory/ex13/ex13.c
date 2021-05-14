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


#define PROCESSES 10


typedef struct{
	int occurences[10];
	char word[10][20];
	char path[10][150];
} shMemStruct;



int make_children(int n) {
    pid_t pid;
    int i;
	pid_t pidF = getpid();
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i;
		else if (getpid() == pidF && i == n - 1)
			return pidF;
    }
    return 0;
}

int main(void) {
	shMemStruct *shMemObj;
	int structSize = sizeof(shMemStruct);
	
	shm_unlink ("/ex13");
	int fd = shm_open("/ex13", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
    if (fd == -1) {
        printf("Error opening shared memory. Please check writer.c!\n");
        exit(EXIT_FAILURE);
    }
	
	ftruncate(fd, structSize);
	shMemObj = (shMemStruct*) mmap(NULL, structSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(shMemObj == NULL) {
		printf("Could not retreive info from shared memory! -Exiting-\n");
		return -1;
	}
	
	
	FILE *lyrics;
	char haystack[1200];
	
	strcpy(shMemObj->word[0], "close");
	strcpy(shMemObj->word[1], "far");
	strcpy(shMemObj->word[2], "heart");
	strcpy(shMemObj->word[3], "Forever");
	strcpy(shMemObj->word[4], "nothing");
	strcpy(shMemObj->word[5], "else");
	strcpy(shMemObj->word[6], "matters");
	strcpy(shMemObj->word[7], "different");
	strcpy(shMemObj->word[8], "cared");
	strcpy(shMemObj->word[9], "what");
	
	int id = make_children(PROCESSES);
	
	int i;
	for (i = 0; i < PROCESSES; i++) {
		sprintf(shMemObj -> path[i], "file%d.txt", i);

		if (id == i) {
			lyrics = fopen(shMemObj->path[i], "r");  //"r" read option
				while(fgets(haystack, sizeof(haystack), lyrics) != NULL){ 
				if(strstr(haystack, shMemObj->word[i]) != NULL){ //Needle in the haystack
					shMemObj->occurences[i]++;
				}
			}
			fclose(lyrics);
			exit(EXIT_SUCCESS);
		}
	}
	
	for (i = 0; i < PROCESSES; i++) {
		wait(NULL);
	}
	
	for (i = 0; i < PROCESSES; i++) {
		printf("Child %d found %d occurrences of word: %s\n", i, shMemObj->occurences[i], shMemObj->word[i]);
	}

    if (munmap((void *) shMemObj, structSize) < 0) {
        printf("Error unmapping at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }

	return 0;
}

