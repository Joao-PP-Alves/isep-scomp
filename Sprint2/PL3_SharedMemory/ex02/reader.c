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

typedef struct {
	int code;
	char description[30];
	int price;
	int canRead;
} product_info;

int main(void){
	int shm_field, close_check;
    int data_size = sizeof(product_info);
	product_info *product;

	shm_field = shm_open("/ex02", O_RDWR, S_IWUSR|S_IRUSR);
	if (shm_field == -1){
        exit(EXIT_FAILURE);
    }
	
	int ftruncate_value = ftruncate(shm_field, data_size);
	if (ftruncate_value == -1) 
        exit(EXIT_FAILURE);
	
	product = (product_info *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, shm_field, 0);
	if (product == MAP_FAILED) 
        exit(EXIT_FAILURE);
	
	printf("\nREADER\n");
    printf("Retrieved product information:\n");
	
	printf("Code: %d\n", product->code);
	
	printf("Description (30 chars max.): %s \n", product->description);
	printf("Price: %d\n", product->price);
	
	close_check = munmap(product, data_size);
	if (close_check < 0) 
        exit(EXIT_FAILURE);
	
	close_check = close(open_check);
	if (close_check < 0) 
        exit(EXIT_FAILURE);

	close_check = shm_unlink("/ex02");
	if (close_check < 0) 
        exit(EXIT_FAILURE);
	
	return 0;
}