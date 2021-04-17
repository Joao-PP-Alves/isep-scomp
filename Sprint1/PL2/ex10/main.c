#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define NUMBER_OF_CHILDREN 1
#define CREDITS 20
#define LEITURA 0
#define ESCRITA 1
#define WIN_BET 10
#define LOSE_BET 5


int make_children(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}


int main(void) {
	
	const int NEXT_BET = 1;
	const int OUT_OF_CREDITS = 0;
	
	printf("Betting system... try to beat it!\n");
		
	int fdWin[2];
	if (pipe(fdWin) == -1) {
        perror("Pipe failed");
        return 1;
    }

	int fdLose[2];
	if (pipe(fdLose) == -1) {
        perror("Pipe failed");
        return 1;
    }
	
	srand(time(0) - 1); // -1 to change random number;

	int id = make_children(NUMBER_OF_CHILDREN);
	
	if (id == 0) {
		
		close(fdWin[ESCRITA]);
        close(fdLose[LEITURA]);
		int currentCredits = CREDITS;
		while (currentCredits > 0) {
			int parentBet = rand() % 5 + 1; 
			write(fdLose[ESCRITA], &NEXT_BET, sizeof(parentBet));
			
			int childBet = 0;
			read(fdWin[LEITURA], &childBet, sizeof(childBet));
			
			if (childBet == parentBet) {
				printf("Child won the bet with: %d, parent bet was: %d\n", childBet, parentBet);
                currentCredits = currentCredits + WIN_BET;
			
			} else {
				printf("Child lost the bet with: %d, parent bet was: %d\n", childBet, parentBet);
                currentCredits = currentCredits - LOSE_BET;
			}		
            printf("--- Current Credit is: %d ---\n", currentCredits);			
		}
		
		write(fdLose[ESCRITA], &OUT_OF_CREDITS, sizeof(OUT_OF_CREDITS));
        close(fdWin[LEITURA]);
        close(fdLose[ESCRITA]);
	}
	
	if (id > 0) {
		srand(time(0) + 1); //+1 to change the random number;
		close(fdWin[LEITURA]);
        close(fdLose[ESCRITA]);
        int continueBetting = 1;
        while (continueBetting == 1) {
            read(fdLose[LEITURA], &continueBetting, sizeof(continueBetting));
            int childBet = rand() % 5 + 1;
            write(fdWin[ESCRITA], &childBet, sizeof(childBet));
        }
        close(fdWin[ESCRITA]);
        close(fdLose[LEITURA]);
        exit(EXIT_SUCCESS);
		
	}


    return 0;
}
