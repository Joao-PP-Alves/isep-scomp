#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>


int main(){
    char signo[20];
    int n, ret;
    printf("pid= ");
    scanf("%d", &n); //scan do tamanho do array
    printf("\nsignal= ");
    scanf("%s", &signo);

    char str1[80];
    strcpy(str1, "kill -");
    strcat(str1, signo);
    char str[80];
    sprintf(str, "%d", n);
    ret = execlp ("kill", str1, str, (char*)NULL);

    exit(ret);
    return 0;
}