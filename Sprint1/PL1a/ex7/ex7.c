#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE 1000
int main ()
{
int numbers[ARRAY_SIZE]; /* array to lookup */
int n; /* the number to find */
time_t t; /* needed to init. the random number generator (RNG)
*/
int i;

/* intializes RNG (srand():stdlib.h; time(): time.h) */
srand ((unsigned) time (&t));

/* initialize array with random numbers (rand(): stdlib.h) */
for (i = 0; i < ARRAY_SIZE; i++)
numbers[i] = rand () % 10000;

/* initialize n */
n = rand () % 10000;

/* variable to count the number of n entries in the array */
int counter = 0;

/* variable with the position of the mid element in the array */
int mid_size = ARRAY_SIZE/2;

/* fork to create child process */
pid_t p = fork();

if (p<0){
    printf("Fork Failed!");
    exit(-1);
}

if (p==0){
    for (i = 0; i <mid_size; i++){
        if (numbers[i] == n){
            counter++;
        }
    }
    exit(counter);
}
for (i = mid_size; i < ARRAY_SIZE; i++){
    if (numbers[i] == n){
        counter++;
    }
}

int child_status;

waitpid(p, &child_status, 0);

if (WIFEXITED(child_status)){
    counter = counter + WEXITSTATUS(child_status);
}

printf("Number of n's in the array: %d\n", counter);

return 0;
}