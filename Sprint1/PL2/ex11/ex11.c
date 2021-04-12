#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>

#define NUMBER_OF_CHILDREN 5
#define NUMBER_OF_PIPES 6

int main(){
    int pipe_field[NUMBER_OF_PIPES][2];

    int i;
    for (i = 0; i < NUMBER_OF_PIPES; i++)
    {
        if (pipe(pipe_field[i]) == -1)
        {
            perror("Pipe failed!");
            exit(-1);
        }
    }

    pid_t pid;
    int reading_variable, random_num;
    for(i=0; i<NUMBER_OF_CHILDREN; i++){
        pid=fork();
        if(pid==-1){
            perror("Fork failed!");
            exit(-1);
        }
        if(pid==0){
            srand((unsigned)time(NULL) * getpid());
            close(pipe_field[i][1]);
            read(pipe_field[i][0], &reading_variable, sizeof(reading_variable));
            random_num = rand() % (1 + 500) + 1;
            printf("pid: %d, random number: %d\n", getpid(), random_num);
            
            close(pipe_field[i][0]);
			close(pipe_field[i+1][0]);

            if(random_num>reading_variable){
                write(pipe_field[i+1][1],&random_num,sizeof(random_num));  
            }else{
                write(pipe_field[i+1][1],&reading_variable,sizeof(reading_variable));
            }

            close(pipe_field[i+1][1]);
			exit(EXIT_SUCCESS);
        }
    }
    srand((unsigned)time(NULL) * getpid());
    random_num = rand() % (1 + 500) + 1;
    printf("(PARENT) pid: %d, random number: %d\n", getpid(), random_num);

    close(pipe_field[0][0]);
	write(pipe_field[0][1],&random_num,sizeof(random_num));
	close(pipe_field[0][1]);

    //Neste caso não é necessário o waitpid porque os pipes já esperam que hajam dados no mesmo para poderem ser lidos
    //e assim os processos filho são obrigados a esperar que hajam dados para resumir a sua leitura e posteriormente
    //colocar o número no pipe para o próximo.
	close(pipe_field[NUMBER_OF_CHILDREN][1]);
	read(pipe_field[NUMBER_OF_CHILDREN][0],&reading_variable,sizeof(reading_variable));
	close(pipe_field[NUMBER_OF_CHILDREN][0]);

	printf("Biggest number: %d\n",reading_variable);
    return 0;
}