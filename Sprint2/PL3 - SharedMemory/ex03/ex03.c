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
	isepInfo array[STRUCTS_NUM];
	isepInfo array2[STRUCTS_NUM];

	int isepInfoSize = sizeof(isepInfo);

	fill_array(array, STRUCTS_NUM);
	
	int processesPipe[2];
	pipe(processesPipe);


	
	pid_t pid = fork();
	int i;
	
	struct timeval instantTime;
	gettimeofday(&instantTime, NULL);
	long startPipe = instantTime.tv_sec * (int)1000000 + instantTime.tv_usec;

	if (pid == 0) {
		for (i = 0; i < STRUCTS_NUM; i++) {
			close(processesPipe[ESCRITA]);
			read(processesPipe[LEITURA], &array2[i], sizeof(array));
			//printf("Chegou ao filho, Array2[%d]: %d\n", i, array2[i].number);
			close(processesPipe[LEITURA]);

		}
		close(processesPipe[LEITURA]);

		exit(EXIT_SUCCESS);

	} else if (pid > 0) {
		for (i = 0; i < STRUCTS_NUM; i++) {
			close(processesPipe[LEITURA]);
			write(processesPipe[ESCRITA], &array[i], sizeof(array));
			//printf("Chegou ao pai\n");
			close(processesPipe[ESCRITA]);

		}
		close(processesPipe[ESCRITA]);
	}

	wait(NULL);
	gettimeofday(&instantTime, NULL);
	long finishPipe = instantTime.tv_sec * (int)1000000 + instantTime.tv_usec;
	double total = (double) (finishPipe - startPipe)/CLOCKS_PER_SEC;
	printf("Pipe took %f to finish being written and read\n", total);
	
	
	
	shm_unlink ("/ex03");
	int fd = shm_open("/ex03", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
    if (fd == -1) {
        printf("Error opening shared memory. Please check writer.c!\n");
        exit(EXIT_FAILURE);
    }
	
	ftruncate(fd, isepInfoSize*STRUCTS_NUM);
	isepInfo *isepInfo1 = mmap(NULL, isepInfoSize*STRUCTS_NUM, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(isepInfo1 == NULL) {
		printf("Could not retreive info from shared memory! -Exiting-\n");
		return -1;
	}
	
	gettimeofday(&instantTime, NULL);
	long startShMem = instantTime.tv_sec * (int)1000000 + instantTime.tv_usec;
	
	for (i = 0; i < STRUCTS_NUM; i++) {
		isepInfo1[i].number = array[i].number;
		strcpy(isepInfo1[i].info, array[i].info);
		

	}
	
	// getting finish time
	//long finishShMemory = instantTime.tv_sec * (int)1e6 + instantTime.tv_usec;

	//printf("Shared Memory took %ld to finish transfer\n", (finishShMemory - start));

	
	
	
		/*
	
	for (i = 0; i < isepInfoSize; i++) {
		isepInfo1[i] = array[i];
	}		
		**/


	
	gettimeofday(&instantTime, NULL);
	long finishShMem = instantTime.tv_sec * (int)1000000 + instantTime.tv_usec;
	total = (double) (finishShMem - startShMem)/CLOCKS_PER_SEC;
	printf("Writer(ex03.c) took %f to finish transfer to shared memory\n", total);


    if (munmap((void *)isepInfo1, isepInfoSize) < 0) {
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
