#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER_SIZE =  100;
    const char* filename = "teste.txt";

    int fd[2];

    if(pipe(fd)==-1){ 
        perror("Pipe failed");
        return 1; 
    }
    
    pid_t pid = fork();
    if(pid==-1){
        perror("fork failed");
        return 1;
    }

    if(pid>0){
		ssize_t read_file;
		size_t size = 0;
		char *line = NULL;
		FILE *filep;
		
        close(fd[LEITURA]);
        filep = fopen(filename, "r");
        if(filep == NULL)
            exit(EXIT_FAILURE);
        while ((read_file = getline(&line, &size, filep)) != -1){
            write(fd[ESCRITA], line, read_file); 
        }
        fclose(filep);
        close(fd[ESCRITA]);
        if(line!=NULL)
            free(line);
        wait(NULL);
    
    }else{
        close(fd[ESCRITA]);
        char buffer[BUFFER_SIZE];
        int n;
        printf("filho recebeu: \n");
        while((n=read(fd[LEITURA], buffer, BUFFER_SIZE-1))){
            buffer[n]= '\0';
            printf("%s", buffer);
        }
		printf("\n");
        close(fd[LEITURA]);
    }
    return 0;
}
