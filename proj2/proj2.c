#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

//semaphores
sem_t *mutex;
sem_t *barrier1;
sem_t *barrier2;
sem_t *oxyQueue;
sem_t *hydroQueue;
sem_t *mutex2;
sem_t *queue_barrier;

//output file
FILE *out;

int sems_init(){
    mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if(mutex == MAP_FAILED){
        return 1;
    }
    else if(sem_init(mutex, 1, 1) == -1){
        return 2;
    }

    oxyQueue = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if(oxyQueue == MAP_FAILED){
        return 1;
    }
    else if(sem_init(oxyQueue, 1, 0) == -1){
        return 2;
    }

    hydroQueue = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if(hydroQueue == MAP_FAILED){
        return 1;
    }
    else if(sem_init(hydroQueue, 1, 0) == -1){
        return 2;
    }

    barrier1 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if(barrier1 == MAP_FAILED){
        return 1;
    }
    else if(sem_init(barrier1, 1, 0) == -1){
        return 2;
    }

    barrier2 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if(barrier2 == MAP_FAILED){
        return 1;
    }
    else if(sem_init(barrier2, 1, 1) == -1){
        return 2;
    }

    mutex2 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if(mutex2 == MAP_FAILED){
        return 1;
    }
    else if(sem_init(mutex2, 1, 1) == -1){
        return 2;
    }

    queue_barrier = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if(queue_barrier == MAP_FAILED){
        return 1;
    }
    else if(sem_init(queue_barrier, 1, 0) == -1){
        return 2;
    }
    return 0;
}

void sems_dest(){
    munmap(mutex, sizeof(sem_t));
    munmap(oxyQueue, sizeof(sem_t));
    munmap(hydroQueue, sizeof(sem_t));
    munmap(barrier1, sizeof(sem_t));
    munmap(barrier2, sizeof(sem_t));
    munmap(mutex2, sizeof(sem_t));
    munmap(queue_barrier, sizeof(sem_t));

    sem_destroy(mutex);
    sem_destroy(oxyQueue);
    sem_destroy(hydroQueue);
    sem_destroy(barrier1);
    sem_destroy(barrier2);
    sem_destroy(mutex2);
    sem_destroy(queue_barrier);
}

int create_shmem(long NO, long NH, int **oxygen, int **hydrogen, int **line_num, int **noM, int **count, int **NH_remaining, int **NO_remaining, int **atoms){
    *NO_remaining = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*NO_remaining == MAP_FAILED){
        return 1;
    }
    **NO_remaining = (int) NO;

    *NH_remaining = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*NH_remaining == MAP_FAILED){
        return 1;
    }
    **NH_remaining = (int) NH;

    *oxygen = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*oxygen == MAP_FAILED){
        return 1;
    }
    **oxygen = 0;

    *hydrogen = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*hydrogen == MAP_FAILED){
        return 1;
    }
    **hydrogen = 0;

    *line_num = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*line_num == MAP_FAILED){
        return 1;
    }
    **line_num = (int)1;

    *noM = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*noM == MAP_FAILED){
        return 1;
    }
    **noM = (int) 1;

    *count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*count == MAP_FAILED){
        return 1;
    }
    **count = 0;

    *atoms = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*atoms == MAP_FAILED){
        return 1;
    }
    **atoms = 0;
    return 0;
}

void clear_shmem(int **oxygen, int **hydrogen, int **line_num, int **noM, int **count, int **NH_remaining, int **NO_remaining, int **atoms) {
    munmap(*oxygen, sizeof(int));
    munmap(*hydrogen, sizeof(int));
    munmap(*line_num, sizeof(int));
    munmap(*noM, sizeof(int));
    munmap(*count, sizeof(int));
    munmap(*NH_remaining, sizeof(int));
    munmap(*NO_remaining, sizeof(int));
    munmap(*atoms, sizeof(int));
}

void ox_queue(int idO, long TI, long TB, int *oxygen, int *hydrogen, int *line_num, int *noM, int *count, const int *NH_remaining, int *NO_remaining, int *atoms){
    srandom(getpid() * time(NULL)); //for truly random number generating

    sem_wait(mutex);
    *atoms += 1;
    sem_post(mutex);
    if(*atoms == (*NH_remaining + *NO_remaining)){
        sem_post(queue_barrier);
    }
    sem_wait(queue_barrier);
    sem_post(queue_barrier);

    sem_wait(mutex);
    fprintf(out,"%d: O %d: started\n", *line_num, idO);
    fflush(out);
    *line_num += 1;
    sem_post(mutex);
    usleep((random()%(TI+1)) * 1000);
    sem_wait(mutex);
    fprintf(out,"%d: O %d: going to queue\n", *line_num, idO);
    fflush(out);
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

    if(*NH_remaining <= 1) {
        fprintf(out,"%d: O %d: not enough H\n", *line_num, idO);
        fflush(out);
        *line_num += 1;
        exit(0);
    }

    sem_wait(mutex2);
    fprintf(out,"%d: O %d: creating molecule %d\n", *line_num, idO, *noM);
    fflush(out);
    *line_num += 1;
    *count += 1;

    if (*count == 3){
        sem_wait(barrier2);
        sem_post(barrier1);
    }
    sem_post(mutex2);

    sem_wait(barrier1);
    sem_post(barrier1);

    sem_wait(mutex2);
    usleep((random()%(TB+1)) * 1000);
    fprintf(out,"%d: O %d: molecule %d created\n", *line_num, idO, *noM);
    fflush(out);
    *line_num += 1;
    *count -= 1;
    if (*count == 0){
        sem_wait(barrier1);
        sem_post(barrier2);
    }
    sem_post(mutex2);

    sem_wait(barrier2);
    sem_post(barrier2);
    *noM += 1;

    *NO_remaining -= 1;

    sem_post(mutex);

    if(*NH_remaining <= 1) {
        for(int i = 0; i < *NO_remaining; i++) {
            sem_post(oxyQueue);
        }
    }
}

void hyd_queue(const int idH, long TI, long TB, int *oxygen, int *hydrogen, int *line_num, const int *noM, int *count, int *NH_remaining, const int *NO_remaining, int *atoms){
    srandom(getpid() * time(NULL)); //for truly random number generating

    sem_wait(mutex);
    *atoms += 1;
    sem_post(mutex);
    if(*atoms == (*NH_remaining + *NO_remaining)){
        sem_post(queue_barrier);
    }
    sem_wait(queue_barrier);
    sem_post(queue_barrier);

    sem_wait(mutex);
    fprintf(out,"%d: H %d: started\n", *line_num, idH);
    fflush(out);
    *line_num += 1;
    sem_post(mutex);
    usleep((random()%(TI+1)) * 1000);
    sem_wait(mutex);
    fprintf(out,"%d: H %d: going to queue\n", *line_num, idH);
    fflush(out);
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

    if(*NH_remaining <= 1 || *NO_remaining < 1){
        fprintf(out,"%d: H %d: not enough O or H\n", *line_num, idH);
        fflush(out);
        *line_num += 1;
        exit(0);
    }

    sem_wait(mutex2);
    fprintf(out,"%d: H %d: creating molecule %d\n", *line_num, idH, *noM);
    fflush(out);
    *line_num += 1;
    *count += 1;
    if(*count == 3){
        sem_wait(barrier2);
        sem_post(barrier1);
    }
    sem_post(mutex2);

    sem_wait(barrier1);
    sem_post(barrier1);

    sem_wait(mutex2);
    usleep((random()%(TB+1)) * 1000);
    fprintf(out,"%d: H %d: molecule %d created\n", *line_num, idH, *noM);
    fflush(out);
    *line_num += 1;
    *count -= 1;
    if(*count == 0){
        sem_wait(barrier1);
        sem_post(barrier2);
    }
    sem_post(mutex2);

    sem_wait(barrier2);
    sem_post(barrier2);

    *NH_remaining -= 1;

    if(*NH_remaining <= 1 || *NO_remaining < 1){
        for(int i = 0; i < *NO_remaining; i++) {
            sem_post(oxyQueue);
        }
        for(int i = 0; i < *NH_remaining; i++) {
            sem_post(hydroQueue);
        }
    }
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
    if (*NO <= 0){
        fprintf(stderr,"Error:  Argument NO cannot be negative.\n");
        return 1;
    }
    //check whether argument NH has correct value
    if (*NH <= 0){
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

    int pokus = 10;
    munmap(&pokus, sizeof(int));

    long NO = 0;
    long NH = 0;
    long TI = 0;
    long TB = 0;

    int idO = 0;
    int idH = 0;

    int *oxygen = NULL;
    int *hydrogen = NULL;
    int *line_num = NULL;
    int *noM = NULL;
    int *count = NULL;
    int *NH_remaining = NULL;
    int *NO_remaining = NULL;
    int *atoms = NULL;

    int err = check_args(argc, argv, &NO, &NH, &TI, &TB);
    if(err){
        return err;
    }

    if ((out = fopen("proj2.out","w")) == NULL){
        fprintf(stderr, "Error: Couldn't open output file.\n");
        return 1;
    }

    setbuf(out, NULL);

    if(create_shmem(NO, NH, &oxygen, &hydrogen, &line_num, &noM, &count, &NH_remaining, &NO_remaining, &atoms) == 1){
        clear_shmem(&oxygen, &hydrogen, &line_num, &noM, &count, &NH_remaining, &NO_remaining, &atoms);
        fprintf(stderr, "Error: Shared memory mapping of variables failed.\n");
        return 1;
    }
    if(sems_init() == 1){
        sems_dest();
        fprintf(stderr, "Error: Semaphore memory mapping failed.\n");
        return 1;
    }
    else if(sems_init() == 2){
        sems_dest();
        fprintf(stderr, "Error: Semaphore initialization failed.\n");
        return 1;
    }


    for (int i = 1; i <= NO; i++){
        idO++;
        pid_t id = fork();
        if(id == 0) {
            ox_queue(idO, TI, TB, oxygen, hydrogen, line_num, noM, count, NH_remaining, NO_remaining, atoms);
            exit(0);
        }
        else if(id == -1){
            fprintf(stderr, "Error: Creating of oxygen failed.\n");
            EXIT_FAILURE;
        }
    }

    for (int i = 1; i <= NH; i++){
        idH++;
        pid_t id = fork();
        if(id == 0) {
            hyd_queue(idH, TI, TB, oxygen, hydrogen, line_num, noM, count, NH_remaining, NO_remaining, atoms);
            exit(0);
        }
        else if(id == -1){
            fprintf(stderr, "Error: Creating of hydrogen failed.\n");
            EXIT_FAILURE;
        }
    }
    while(wait(NULL) > 0);

    fclose(out);
    clear_shmem(&oxygen, &hydrogen, &line_num, &noM, &count, &NH_remaining, &NO_remaining, &atoms);
    sems_dest();

    return 0;
}
