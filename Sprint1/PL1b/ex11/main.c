#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define CMD_TAM 32
#define NMR_STRUCS 5
#define TIME_LIMIT 11
#define CMD_SIZE 14

typedef struct {
	char cmd[CMD_TAM];
	int tempo;
} comando;

volatile sig_atomic_t cmdNr = 0;
volatile sig_atomic_t sonPid = -1;
char executable[CMD_SIZE] = "/0";

void handle_ALRM(int signo, siginfo_t *sinfo, void *context) {
    char msg[CMD_SIZE] = "/0";
    sprintf(msg, "Gonna kill %d\n", sonPid);
    write(STDOUT_FILENO, msg, (sizeof(msg)+1));
	
	strncpy(msg, "/0", (sizeof(msg)+1));
	//msg = "/0";
	kill(sonPid, SIGKILL);
    wait(NULL);
    sprintf(msg, "The command %s didn’t end in its allowed time!\n", executable);
    write(STDOUT_FILENO, msg, (sizeof(msg)+1));
}

void handleCHLD(int signo, siginfo_t *sinfo, void *context) {

    alarm(0);
}


void fillCommandsList(comando *cmds) {

    int i;
    char buffer[CMD_SIZE] = "/0";
    int t_exec;
	//int t_allowed;
	
    srand(time(0));

	for (i = 0; i < NMR_STRUCS; i++) {
		t_exec = rand() % 10;
		sprintf(buffer, "dorminhoco %d", t_exec);
        strncpy(cmds[i].cmd, buffer, (sizeof(buffer)+1));
        cmds[i].tempo = rand() % 10;
    }
}

int main() {
	struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGCHLD);
    act.sa_sigaction = handle_ALRM;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, &act, NULL);
	
	struct sigaction act2;
    memset(&act2, 0, sizeof(struct sigaction));
    sigemptyset(&act2.sa_mask);
    act2.sa_sigaction = handleCHLD;
    act2.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &act2, NULL);
	
	printf("Father process: %d\n", getpid());

	comando c1, c2, c3, c4, c5;

    comando commandsList[NMR_STRUCS] = {c1, c2, c3, c4, c5};
	fillCommandsList(commandsList);

	int i;
	for (i = 0; i < NMR_STRUCS; i++) {

        int j;		
		pid_t pid = fork();
			if(pid==-1){
			perror("fork failed");
			return 1;
		}

		if (pid == 0) {
			sonPid = getpid();
			execl(commandsList[i].cmd, commandsList[i].cmd, (char *)NULL);
            //printf("Process %s on child failed.", commandList[i].cmd);
            //exit(EXIT_FAILURE);
		
		} if (pid > 0) {
			strncpy(executable, commandsList[i].cmd, (sizeof(commandsList[i].cmd)+1));
			alarm(commandsList[i].tempo);
		}
		
        for (j = 0; j < commandsList[i].tempo; j++) {
			
            if((j + 1) > TIME_LIMIT) {

                cmdNr = (i + 1);
                //raise(SIGINT);
                break;
            }
            sleep(1);
        }
    }

	
    return 0;
}





