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
    printf("WRITTER\n");
    int open_check, size, close_check;
    size = sizeof(sample_struct);

    sample_struct *struct1;
    shm_unlink("/ex04");
    open_check = shm_open("/ex04", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);  
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
        
    int i;
    char aux[NUMBER_OF_LETTERS];
    for(i = 0; i < NUMBER_OF_LETTERS; i++) {
        char random_letter = 'A' + (random() % 26);
        aux[i] = random_letter;
        printf("letter sent -> %c\n",aux[i]);
    }    
    strcpy(struct1->letters,aux);   

    close_check = munmap((void *) struct1, size);
	if (close_check < 0){
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }
	
	close_check = close(open_check);
	if (close_check < 0){
        perror("close failed");
        exit(EXIT_FAILURE);
    }

	printf("FIM\n");

    return 0;
}