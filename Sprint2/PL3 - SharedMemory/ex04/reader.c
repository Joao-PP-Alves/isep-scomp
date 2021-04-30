#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>

#define NUMBER_OF_LETTERS 100

typedef struct {
	char letters[NUMBER_OF_LETTERS];
} sample_struct;

int main(){

    printf("READER\n");
    int open_check, size, close_check;
    size = sizeof(sample_struct);

    sample_struct *struct1;

    open_check = shm_open("/ex04", O_RDWR, S_IWUSR|S_IRUSR);
	if (open_check == -1){
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
	
	int ftruncate_value = ftruncate(open_check, size);
	if (ftruncate_value == -1){
        perror("ft failed");
        exit(EXIT_FAILURE);
    }

	struct1 = (sample_struct *) mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, open_check, 0);
	if (struct1 == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    int sum = 0,i;
    char aux[NUMBER_OF_LETTERS];
    strcpy(aux,struct1->letters);
    for (i=0; i< NUMBER_OF_LETTERS; i++){
        printf("letter recieved -> %c\n", aux[i]);
        sum += (int)aux[i];
    }

    sum = sum/NUMBER_OF_LETTERS;

    printf("Average -> %d\n",sum);

    close_check = munmap(struct1, size);
	if (close_check < 0){
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    close_check = close(open_check);
	if (close_check < 0){
        perror("close failed");
        exit(EXIT_FAILURE);
    }

	close_check = shm_unlink("/ex04");
	if (close_check < 0){
        perror("unlink failed");
        exit(EXIT_FAILURE);
    }

    printf("FIM\n");    
	
    return 0;
}