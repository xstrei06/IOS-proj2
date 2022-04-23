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
#include <sys/shm.h>

//semaphores
sem_t *mutex;
sem_t *barrier;
sem_t *oxyQueue;
sem_t *hydroQueue;
sem_t *mutex2;

FILE *file;

void sems_init(sem_t **mutex, sem_t **oxyQueue, sem_t **hydroQueue, sem_t **barrier, sem_t **mutex2){
    *mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    sem_init(*mutex, 1, 1);

    *oxyQueue = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    sem_init(*oxyQueue, 1, 0);

    *hydroQueue = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    sem_init(*hydroQueue, 1, 0);

    *barrier = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    sem_init(*barrier, 1, 3);

    *mutex2 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    sem_init(*mutex2, 1, 1);
}

void ox_code(int *oxygen, int *hydrogen, int *line_num, int idO, int TI, int *noM){
    sem_wait(mutex);
    //fprintf(file, "%d: O %d: started", *line_num, idO);
    printf("%d: O %d: started\n", *line_num, idO);
    *line_num += 1;
    usleep((random()%(TI+1)) * 1000);
    printf("%d: O %d: going to queue\n", *line_num, idO);
    *line_num += 1;
    sem_post(mutex);

    sem_wait(mutex);
    *oxygen += 1;
    if (*hydrogen >= 2){
        sem_post(hydroQueue);
        sem_post(hydroQueue);
        *hydrogen -= 2;
        sem_post(oxyQueue);
        *oxygen -= 1;
    }
    else{
        sem_post(mutex);
    }
    sem_wait(oxyQueue);
    sem_wait(mutex2);
    printf("%d: H %d: creating molecule %d", *line_num, idO, *noM);
    *line_num += 1;
    sem_post(mutex2);

    //TODO bond();

    sem_wait(barrier);
    sem_post(mutex);
}

void hyd_code(int *oxygen, int *hydrogen, int *line_num, const int idH, int TI, int *noM){
    sem_wait(mutex);
    //fprintf(file, "%d: H %d: started", *line_num, *idH);
    printf("%d: H %d: started\n", *line_num, idH);
    *line_num += 1;
    usleep((random()%(TI+1)) * 1000);
    printf("%d: H %d: going to queue\n", *line_num, idH);
    *line_num += 1;
    sem_post(mutex);

    sem_wait(mutex);
    *hydrogen += 1;
    if (*hydrogen >=2 && *oxygen  >= 1){
        sem_post(hydroQueue);
        sem_post(hydroQueue);
        *hydrogen -= 2;
        sem_post(oxyQueue);
        *oxygen -= 1;
    }
    else{
        sem_post(mutex);
    }
    sem_wait(hydroQueue);
    sem_wait(mutex2);
    printf("%d: H %d: creating molecule %d", *line_num, idH, *noM);
    *line_num += 1;
    sem_post(mutex2);

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

    int *oxygen = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *oxygen = 0;
    int idO = 0;

    int *hydrogen = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *hydrogen = 0;
    int idH = 0;

    int *line_num = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *line_num = 1;

    int *noM = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *noM = 0;

    int err = check_args(argc, argv, &NO, &NH, &TI, &TB);
    if(err){
        return err;
    }

    if ((file = fopen("proj2.out","w")) == NULL){
        fprintf(stderr, "Error: Couldn't open output file.\n");
        return 1;
    }

    sems_init(&mutex, &oxyQueue, &hydroQueue, &barrier, &mutex2);

    for (int i = 1; i <= NO; i++){
        idO++;
        pid_t id = fork();
        if(id == 0) {
            ox_code(oxygen, hydrogen, line_num, idO, TI, noM);
            //printf("picovina toto nechapem tomu %d\n", getpid());
            //printf("%d\n", *line_num);
            exit(0);
        }
    }
    while(wait(NULL) > 0);

    for (int i = 1; i <= NH; i++){
        idH++;
        pid_t id = fork();
        if(id == 0) {
            hyd_code(oxygen, hydrogen, line_num, idH, TI, noM);
            //printf("hento vodiky prijebane tu kokotiny robi %d\n", getpid());
            exit(0);
        }
    }
    while(wait(NULL) > 0);

    return 0;
}