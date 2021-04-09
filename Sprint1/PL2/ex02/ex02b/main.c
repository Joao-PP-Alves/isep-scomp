#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
    const int LEITURA = 0;
    const int ESCRITA = 1;

    int fd[2];

    struct S1 {
        int i;
        char str[100];
    };
	
	struct S1 info;

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
        close(fd[LEITURA]);
        printf("PAI:Insira o numero\n");
        scanf("%d", &info.i);
        printf("PAI:Insira a string\n");
        scanf("%s", info.str);
        write(fd[ESCRITA], &info, sizeof(info)); 
        close(fd[ESCRITA]);
    }

    if(pid==0){	
        close(fd[ESCRITA]);
        read(fd[LEITURA], &info, 100);
        close(fd[LEITURA]);
        printf("filho: numero %d\n", info.i);
        printf("filho: string %s\n", info.str);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
