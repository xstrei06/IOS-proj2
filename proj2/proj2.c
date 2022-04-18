#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

//variables
int line_num = 1;
sem_t *mutex;
int oxygen  = 0;
int hydrogen = 0;
sem_t *barrier;
sem_t *oxyQueue;
sem_t *hydroQueue;
FILE *file;

void initialize_semaphores(){
    sem_init(mutex, 1, 1);
    sem_init(barrier, 1, 3);
    sem_init(oxyQueue, 1, 0);
    sem_init(hydroQueue, 1, 0);
}

void ox_code(){
    sem_wait(mutex);
    oxygen++;
    if (hydrogen >= 2){
        sem_post(hydroQueue);
        sem_post(hydroQueue);
        hydrogen -= 2;
        sem_post(oxyQueue);
        oxygen--;
    }
    else{
        sem_post(mutex);
    }
    sem_wait(oxyQueue);
    //TODO bond();

    sem_wait(barrier);
    sem_post(mutex);
}

void hyd_code(){
    sem_wait(mutex);
    hydrogen++;
    if (hydrogen >=2 && oxygen  >= 1){
        sem_post(hydroQueue);
        sem_post(hydroQueue);
        hydrogen -= 2;
        sem_post(oxyQueue);
        oxygen--;
    }
    else{
        sem_post(mutex);
    }
    sem_wait(hydroQueue);
    //TODO bond();

    sem_wait(barrier);
}

int check_args(int argc, char *argv[], long *NO, long *NH, long *TI, long *TB){
    //check whether correct number of arguments was passed
    if (argc != 5){
        fprintf(stderr,"Error:  Wrong number of arguments.\n");
        return 1;
    }
    //"bin" variables to store non-numerical characters in parameters
    char *o;
    char *h;
    char *i;
    char *b;

    //getting numbers from passed arguments
    *NO = strtol((const char*) argv[1], &o, 10);
    *NH = strtol((const char*) argv[2], &h, 10);
    *TI = strtol((const char*) argv[3], &i, 10);
    *TB = strtol((const char*) argv[4], &b, 10);

    //check whether argument NO has correct value
    if (*NO < 0){
        fprintf(stderr,"Error:  Argument NO cannot be negative.\n");
        return 1;
    }
    //check whether argument NH has correct value
    if (*NH < 0){
        fprintf(stderr,"Error:  Argument NH cannot be negative.\n");
        return 1;
    }
    //check whether argument TI has correct value
    if (*TI < 0 || *TI > 1000){
        fprintf(stderr,"Error:  Argument TI has to belong to interval <0;1000>.\n");
        return 1;
    }
    //check whether argument TB has correct value
    if (*TB < 0 || *TB > 1000){
        fprintf(stderr,"Error:  Argument TB has to belong to interval <0;1000>.\n");
        return 1;
    }
    //check whether the arguments are only numbers
    if(*o != '\0' || *h != '\0' || *i != '\0' || *b != '\0') {
        fprintf(stderr, "Error: Arguments can contain only numerical values.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {

    long NO = 0;
    long NH = 0;
    long TI = 0;
    long TB = 0;

    int err = check_args(argc, argv, &NO, &NH, &TI, &TB);
    if(err){
        return err;
    }

    if ((file = fopen("proj2.out","w")) == NULL){
        fprintf(stderr, "Error: Couldn't open output file.\n");
        return 1;
    }

    for (int i = 1; i <= NO; i++){
        pid_t id = fork();
        if(id == 0) {
            printf("picovina toto nechapem tomu %d\n", getpid());
            //ox_code();
            exit(0);
        }
    }

    //printf("jo, tak takhle nejak to je %d\n", getpid());
    //printf("NO is: %ld\n", NO);
    //printf("NH is: %ld\n", NH);
    //printf("TI is: %ld\n", TI);
    //printf("TB is: %ld\n", TB);

    return 0;
}