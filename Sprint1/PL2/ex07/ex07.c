#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>


#define NUMBER_OF_CHILDS 5
#define VEC_SIZE 1000
#define VEC_SIZE_FOR_EACH 200
int main(){

    time_t t;
    pid_t p, pids[NUMBER_OF_CHILDS];
    int vec1[VEC_SIZE], vec2[VEC_SIZE], result[VEC_SIZE];
    int status, i, j,k, aux, aux2, counter = 0;
    //1
    int pipe_field[5][2];

    srand ((unsigned) time (&t));

	for (i = 0; i < VEC_SIZE; i++){
		vec1[i] = rand() % 10;		
		vec2[i] = rand() % 10;
	}

    for (i=0; i<NUMBER_OF_CHILDS; i++){
        if (pipe(pipe_field[i]) == -1){
            perror("Pipe failed!");
            exit(1);
        }
    }


    for (i=0;i<NUMBER_OF_CHILDS;i++){
        p = fork();
        if (p<0){
            perror("Fork Failed!");
            exit(-1);
        } else if (p==0){
            close(pipe_field[i][0]);
			
			int low_limit = VEC_SIZE_FOR_EACH * i;		
			int high_limit = (i+1) * VEC_SIZE_FOR_EACH;
			
			for (j = low_limit; j < high_limit; j++){
				aux = vec1[j] + vec2[j];
				write(pipe_field[i][1], &aux, sizeof(int));
                printf("sum[%d] = %d\n",j, aux);
			}
            printf("-------------------------------------\n");
			close(pipe_field[i][1]);
			exit(i);
        } else {
            pid_t pid = wait(&status);
            if (WIFEXITED(status)) {
                pids[i] = pid;
            }
        }
    }
    for (i=0; i<NUMBER_OF_CHILDS; i++){
        if (pids[i]>0){
            counter++;
        }
        if (counter == 5){
            for (j=0; j<NUMBER_OF_CHILDS; j++){
                close(pipe_field[j][1]);
			
		        int low_limit = VEC_SIZE_FOR_EACH * j;		
		        int high_limit = (j+1) * VEC_SIZE_FOR_EACH;
			
		        for(k = low_limit; k < high_limit; k++){
			        read(pipe_field[j][0], &aux2, sizeof(int));
			        result[k] = aux2;
                    printf("result[%d] = %d\n",k, result[k]);	
		        }
		    close(pipe_field[j][0]);
        }
    }


    }
    return 0;
}