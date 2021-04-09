#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>



int main(void) {
    const int LEITURA = 0;
    const int ESCRITA = 1;
    
    char str[100];
    int num;
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
        close(fd[LEITURA]);
        printf("Processo PAI:introduza um numero\n");
        scanf("%d", &num);
        printf("Processo PAI:introduza uma string\n");
        scanf("%s", &str);
        write(fd[ESCRITA], &num, sizeof(num)); 
        write(fd[ESCRITA], &str, sizeof(str)+1); 
        close(fd[ESCRITA]);
    }

    if(pid==0){
        close(fd[ESCRITA]);
        read(fd[LEITURA], &num, sizeof(num));
        read(fd[LEITURA], &str, sizeof(str)+1);
        close(fd[LEITURA]);
        printf("Processo Filho: o pai introduziu o numero %d\n", num);
        printf("Processo Filho: o pai introduziu a string %s\n", str);
        exit(0);
    }
    return 0;
}
