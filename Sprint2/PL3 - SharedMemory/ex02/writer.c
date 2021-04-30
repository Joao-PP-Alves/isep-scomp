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
} product_info;

int main(void){
	int shm_field, close_check;
    int data_size = sizeof(product_info);
	product_info *product;
	
	shm_unlink("/ex02");
	
	shm_field = shm_open("/ex02", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if (shm_field == -1) 
        exit(1);
	
	int ftruncate_value = ftruncate(shm_field, data_size);
	if (ftruncate_value == -1) 
        exit(1);
	
	product = (product_info *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, shm_field, 0);
	if (product == MAP_FAILED) 
        exit(1);
		
	printf("WRITER\n");
    printf("Fill the following product informtion:\n");
	
	printf("Code: \n");
	scanf("%d", &product->code);
	
	printf("Description (30 chars max.): \n");
	scanf("%s", product->description);
	
	printf("Price: \n");
	scanf("%d", &product->price);
	
	close_check = munmap((void *) product, data_size);
	if (close_check < 0) 
        exit(1);
	
	close_check = close(shm_field);
	if (close_check < 0) 
        exit(1);
	printf("FIM\n");
	return 0;
}