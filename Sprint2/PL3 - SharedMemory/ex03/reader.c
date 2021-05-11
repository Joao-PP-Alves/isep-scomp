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
	int number;
	char info[20];
}isepInfo;

#define STRUCTS_NUM 100000
#define LEITURA 0
#define ESCRITA 1

void fill_array(isepInfo *array, int size){
    int i;
	int j = 0;
    for (i = 0; i < size; i++) {
		array[i].number = i;
		char temp[] = "ISEP-SCOMP 2020";
		strcpy(array[i].info, temp);
		j++;
		//printf("Chegou ao filho, Array[%d]: %d\n", i, array[i].number);

	}
}

int main(void) {
	int isepInfoSize = sizeof(isepInfo);
	isepInfo array[STRUCTS_NUM];
	
	int fd = shm_open("/ex03", O_RDWR, S_IRUSR|S_IWUSR);
	
    if (fd == -1) {
        printf("Error opening shared memory. Please check writer.c!\n");
        exit(EXIT_FAILURE);
    }
	
	ftruncate(fd, isepInfoSize*STRUCTS_NUM);
	isepInfo *isepInfo2 = mmap(NULL, isepInfoSize*STRUCTS_NUM, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	struct timeval instantTime;
	gettimeofday(&instantTime, NULL);
	long startShMem = instantTime.tv_sec * (int)1000000 + instantTime.tv_usec;
	
	int i;
	for (i = 0; i < STRUCTS_NUM; i++) {
		array[i].number = isepInfo2[i].number;
		strcpy(array[i].info, isepInfo2[i].info);
		/*
		printf("Number: %d\n", array[i].number);
		printf("Info: %s\n", array[i].info);
*/
	}
	
	gettimeofday(&instantTime, NULL);
	long finishShMem = instantTime.tv_sec * (int)1000000 + instantTime.tv_usec;
	double total = (double) (finishShMem - startShMem)/CLOCKS_PER_SEC;
	printf("Reader took %f to finish reading from shared memory\n", total);


	
	if (munmap((void *)isepInfo2, isepInfoSize*STRUCTS_NUM) < 0) {
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
