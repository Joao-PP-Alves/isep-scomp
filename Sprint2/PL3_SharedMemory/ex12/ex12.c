#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define STR_SIZE 50
#define NR_DISC 10
typedef struct{
	int numero;
	char nome[STR_SIZE];
	int disciplinas[NR_DISC];
	int permissao;
} aluno;

int main(void) {
	int shm_field, i = 0;
	int data_size = sizeof(aluno);

	shm_unlink("/ex12");

	shm_field = shm_open("/ex12",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }
	aluno * shared_data = (aluno*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	
	/* Definir o valor a 100 */
	shared_data->permissao = 0;

    pid_t p = fork();
    if (p == -1){
        perror("Fork Failed!");
        exit(EXIT_FAILURE);
    }
    if (p > 0){
        printf("Number: \n");
		scanf("%d", &shared_data->numero);
	
		printf("Name: \n");
		scanf("%s", shared_data->nome);

		for (i=0;i<NR_DISC;i++){
			printf("Discipline grade: \n");
			scanf("%d", &shared_data->disciplinas[i]);
		}

		shared_data->permissao = 1;
    } else {
        while (!shared_data->permissao);
		int numero;
		char *nome;
        int disc[NR_DISC];
		numero = shared_data->numero;
		nome = shared_data->nome;
        for(i = 0; i < NR_DISC; i++){
            disc[i] = shared_data->disciplinas[i];
        }
		int highest=-1000000;
		int lowest=1000000;
		int average=0;
		for (i=0;i<NR_DISC;i++){
			if(disc[i]>highest){
				highest=disc[i];
			}
			if(disc[i]<lowest){
				lowest=disc[i];
			}
			average+=disc[i];
		}

		average = average/NR_DISC;

		printf("=======================\nStudent Info:\n");
		printf("Name = %s\n", nome);
		printf("Number = %d\n", numero);
		for (i=0; i<NR_DISC;i++){
			printf("Discipline %d = %d\n", i, disc[i]);
		}
		printf("Highest = %d\n", highest);
		printf("Lowest = %d\n", lowest);
		printf("Average = %d\n", average);
		printf("=======================\n");

		if(munmap(shared_data, data_size) == -1){
			perror("Munmap failed.\n");
			exit(EXIT_FAILURE);
		}	

		if(close(shm_field) == -1){
			perror("Cant close object.\n");
			exit(EXIT_FAILURE);
		}
        exit(EXIT_SUCCESS);
    }

	int status;
    wait(&status);

	if(munmap(shared_data, data_size) == -1){
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	

	if(close(shm_field) == -1){	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	if (shm_unlink("/ex12") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }

}