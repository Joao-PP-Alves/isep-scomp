#include <stdio.h>
#include <sys/types.h>

int main(void){
    int x=1;

    pid_t p = fork();
   
    if (p==0){
        sleep(1);
        printf("I'll never join you!\n");
    } else {
        printf("I'm..\n");
        p = fork();
        if (p==0){
            sleep(1);
            printf("I'll never join you!\n");
        } else {
            printf("the..\n");
            p = fork();
            if (p==0){
                sleep(1);
                printf("I'll never join you!\n");
            } else {
                printf("father!\n"); 
            }
        }
    }
return 0;
}