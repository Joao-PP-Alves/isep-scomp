#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//Definido no enunciado
#define BUFFER_SIZE 256

int main() {
    
    int up_pipe[2], down_pipe[2];

    char stdin_channel[BUFFER_SIZE];
    char stdout_channel[BUFFER_SIZE];

    int status;     
    if((pipe(up_pipe) == -1) || (pipe(down_pipe) == -1)) {
		perror("Pipe Failed!");
		return -1;
	}
    pid_t pid = fork();
    if (pid > 0) {
		close(up_pipe[1]); //Fechar escrita UP
        close(down_pipe[0]); //Fechar leitura DOWN

		read(up_pipe[0], stdout_channel, BUFFER_SIZE);
		close(up_pipe[0]);

        char *string;
        string = stdout_channel;

        //Conversão de String
        //Foram usados os valores Ascii de A,Z,a,z -> 65,90,97,122
		while (*string) {
            if (*string >= 65 && *string <= 90) { 
                *string = *string + 32;
            }
            else if (*string >= 97 && *string <= 122) {
                *string = *string - 32;
            }
            string++;
        }

		write(down_pipe[1], stdout_channel, strlen(stdout_channel) + 1);
		close(down_pipe[1]);

    } else if (pid == 0) {
		close(up_pipe[0]); //Fechar leitura UP
        close(down_pipe[1]); //Fechar escrita DOWN

		printf("Escreva alguma coisa até 255 caracteres:\n");
		scanf("%[^\n]s", stdin_channel);

		write(up_pipe[1], stdin_channel, strlen(stdin_channel) + 1);
		close(up_pipe[1]);

		
		read(down_pipe[0], stdin_channel, BUFFER_SIZE);
		close(down_pipe[0]);

		printf("Nova palavra até 255 caracteres: %s\n", stdin_channel);
        
    } else {
        perror("Fork failed!");
        exit(1);
    }

    return 0;
}