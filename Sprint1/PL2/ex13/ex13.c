#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>

#define NUMBER_OF_MACHINES 4
#define NUMBER_OF_PIPES 4 //Number of data transfers
#define NUMBER_OF_PIECES 1000
#define PIECES_M1_TO_M2 5
#define PIECES_M2_TO_M3 5
#define PIECES_M3_TO_M4 10
#define PIECES_M4_TO_A1 100
#define NOTIFICATION_IN 1
#define NOTIFICATION_SENT 0

int main(){
    int i, pipe_field[NUMBER_OF_PIPES][2];
    int notification=1, pieces=0, total_pieces=0, pieces_recieved=0;
    pid_t pids[NUMBER_OF_MACHINES];
    for (i=0; i<NUMBER_OF_PIPES;i++){
        if (pipe(pipe_field[i]) == -1){
            perror("Pipe failed!");
            exit(-1);
        }
    }

    for (i=0;i<NUMBER_OF_MACHINES; i++){
        pids[i] = fork();
        if (pids[i]==0){
            close(pipe_field[i][0]);
            if (i>0){
                close(pipe_field[i-1][1]);
            }
            while(total_pieces<NUMBER_OF_PIECES) {
                if (i==0){
                    if (PIECES_M1_TO_M2 == pieces){
                        notification = NOTIFICATION_IN;
                        total_pieces+=pieces;
                        write(pipe_field[i][1], &notification, sizeof(notification));
                        write(pipe_field[i][1], &pieces, sizeof(pieces));
                        printf("[Sent %d pieces to M2 - %d] \n ",pieces,total_pieces);
                        notification = NOTIFICATION_SENT;
                        pieces = 0;
                    }
                    pieces++;

                } else {
                    read(pipe_field[i-1][0], &notification, sizeof(notification));
                    if (notification == NOTIFICATION_IN){
                        read(pipe_field[i-1][0], &pieces_recieved, sizeof(pieces));
                        pieces+=pieces_recieved;
                        if (i==1){
                            if (PIECES_M2_TO_M3 == pieces){
                                notification = NOTIFICATION_IN;
                                total_pieces+=pieces;
                                write(pipe_field[i][1], &notification, sizeof(notification));
                                write(pipe_field[i][1], &pieces, sizeof(pieces));
                                printf("[Sent %d pieces to M3 - %d] \n ",pieces,total_pieces);
                                notification = NOTIFICATION_SENT;
                                pieces = 0;
                            }
                        }
                        if (i==2){
                            if (PIECES_M3_TO_M4 == pieces){
                                notification = NOTIFICATION_IN;
                                total_pieces+=pieces;
                                write(pipe_field[i][1], &notification, sizeof(notification));
                                write(pipe_field[i][1], &pieces, sizeof(pieces));
                                printf("[Sent %d pieces to M4 - %d] \n ",pieces,total_pieces);
                                notification = NOTIFICATION_SENT;
                                pieces=0;
                            }
                        }
                        if (i==3){
                            if (PIECES_M4_TO_A1 == pieces){
                                notification = NOTIFICATION_IN;
                                total_pieces+=pieces;
                                write(pipe_field[i][1], &notification, sizeof(notification));
                                write(pipe_field[i][1], &pieces, sizeof(pieces));
                                printf("[Sent %d pieces to A1 - %d] \n ",pieces,total_pieces);
                                notification = NOTIFICATION_SENT;
                                pieces=0;
                            }
                        }
                    }
                }
            }
            printf("\n\n\n ----------- Reached here : M%d with pid %d ---------- \n\n\n", i+1, getpid());
            close(pipe_field[i][1]);
            exit(0);
        }
    }
    close(pipe_field[3][1]);
    while(pieces!=NUMBER_OF_PIECES){
        read(pipe_field[3][0], &notification, sizeof(notification));
        if (notification == NOTIFICATION_IN){
            read(pipe_field[3][0], &total_pieces, sizeof(total_pieces));
            pieces+=total_pieces;
        }
    }
    printf("Got all pieces :)!\n");
    close(pipe_field[3][0]);



    return 0;
}