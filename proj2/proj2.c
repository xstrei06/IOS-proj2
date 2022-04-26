/**
 * @file proj2.c
 * @author Jaroslav Streit xstrei06
 * @brief FIT VUT IOS project 2, Building H2O
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

//declaring semaphores for further use
sem_t *mutex;
sem_t *barrier1;
sem_t *barrier2;
sem_t *oxyQueue;
sem_t *hydroQueue;
sem_t *mutex2;
sem_t *queue_barrier;

//output file
FILE *out;

/**
 * @brief Function for mapping shared memory for semaphores and initializing them
 * @return returns 1 if any memory mapping failed or 2 when initialization of any semaphore failed
 */

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

/**
 * @brief function for clearing shared semaphore memory and destroying semaphores
 */

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

/**
 * @brief Function mapping shared memory for shared variables, initializing them and checking for errors
 * @param NO
 * @param NH
 * @param oxygen
 * @param hydrogen
 * @param line_num
 * @param noM
 * @param count
 * @param NH_remaining
 * @param NO_remaining
 * @param atoms
 * @return returns 1 if any memory mapping failed, otherwise returns 0
 */

int create_shmem(long NO, long NH, int **oxygen, int **hydrogen, int **line_num, int **noM, int **count, int **NH_remaining, int **NO_remaining, int **atoms, int **commit_genocide){
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

    *commit_genocide = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(*atoms == MAP_FAILED){
        return 1;
    }
    **commit_genocide = 0;

    return 0;
}

/**
 * @brief Function clearing memory of shared variables
 * @param oxygen
 * @param hydrogen
 * @param line_num
 * @param noM
 * @param count
 * @param NH_remaining
 * @param NO_remaining
 * @param atoms
 * @param commit_genocide
 */

void clear_shmem(int **oxygen, int **hydrogen, int **line_num, int **noM, int **count, int **NH_remaining, int **NO_remaining, int **atoms, int **commit_genocide) {
    munmap(*oxygen, sizeof(int));
    munmap(*hydrogen, sizeof(int));
    munmap(*line_num, sizeof(int));
    munmap(*noM, sizeof(int));
    munmap(*count, sizeof(int));
    munmap(*NH_remaining, sizeof(int));
    munmap(*NO_remaining, sizeof(int));
    munmap(*atoms, sizeof(int));
    munmap(*commit_genocide, sizeof(int));
}

/**
 * @brief Function simulating oxygen queue
 * @param idO id of hydrogen process
 * @param TI maximum time in ms before atom joins the queue
 * @param oxygen number of oxygens in queue
 * @param hydrogen number of hydrogens in queue
 * @param line_num line number (number of action)
 * @param noM molecule id
 * @param count number of atoms in process of creating a molecule (min 0 max 3)
 * @param NH_remaining remaining number of hydrogen atoms
 * @param NO_remaining remaining number of oxygen atoms
 * @param atoms total number of created processes
 * @param commit_genocide flag for failed fork and terminating all children
 */

void ox_queue(int idO, long TI, long TB, int *oxygen, int *hydrogen, int *line_num, int *noM, int *count, const int *NH_remaining, int *NO_remaining, int *atoms, const int *commit_genocide){
    srandom(getpid() * time(NULL)); //for truly random number generating

    //barrier waiting for all processes to be created before starting
    sem_wait(mutex2);
    *atoms += 1;
    sem_post(mutex2);
    if(*atoms == (*NH_remaining + *NO_remaining)){
        sem_post(queue_barrier);
    }
    sem_wait(queue_barrier);
    sem_post(queue_barrier);

    if(*commit_genocide == 1){ //if fork failed, kill all children
        exit(1);
    }

    //start of oxygen and joining the queue
    sem_wait(mutex2);
    fprintf(out,"%d: O %d: started\n", *line_num, idO);
    fflush(out);
    *line_num += 1;
    sem_post(mutex2);
    usleep((random()%(TI+1)) * 1000); //oxygen waits for random time before joining the queue
    sem_wait(mutex2);
    fprintf(out,"%d: O %d: going to queue\n", *line_num, idO);
    fflush(out);
    *line_num += 1;
    sem_post(mutex2);

    //if there are at least 1 atom of oxygen and 2 atoms of hydrogen in the queue, they start creating a molecule
    sem_wait(mutex);
    *oxygen += 1;
    if (*hydrogen >= 2){ //if there is enough atoms to create a molecule, release them from queue
        sem_post(hydroQueue);
        sem_post(hydroQueue);
        *hydrogen -= 2;
        sem_post(oxyQueue);
        *oxygen -= 1;
    }
    else{ //if there is not yet enough atoms in the queue needed for creating a molecule, release mutex and wait for more atoms
        sem_post(mutex);
        //if there is not enough atoms to create a complete molecule, release all remaining atoms from the queue
        //needed in case of input of 1 hydrogen and 1 oxygen only
        if(*NH_remaining <= 1 || *NO_remaining < 1){
            for(int i = 0; i < *NO_remaining; i++) {
                sem_post(oxyQueue);
            }
            for(int i = 0; i < *NH_remaining; i++) {
                sem_post(hydroQueue);
            }
        }
    }

    sem_wait(oxyQueue); //queue of oxygen atoms waiting to be released to create a molecule

    if(*NH_remaining <= 1) { //checks, whether there is enough atoms of hydrogen to create a molecule
                             //if not, prints out status information and process ends
        sem_wait(mutex2);
        fprintf(out,"%d: O %d: not enough H\n", *line_num, idO);
        fflush(out);
        *line_num += 1;
        sem_post(mutex2);
        exit(0);
    }

    //reusable barrier for creating a molecule
    sem_wait(mutex2);
    fprintf(out,"%d: O %d: creating molecule %d\n", *line_num, idO, *noM);
    fflush(out);
    *line_num += 1;
    *count += 1;
    if (*count == 3){ //wait for all 3 atoms to create the molecule
        sem_wait(barrier2);
        sem_post(barrier1);
    }
    sem_post(mutex2);

    sem_wait(barrier1);
    sem_post(barrier1);

    sem_wait(mutex2);
    usleep((random()%(TB+1)) * 1000); //oxygen waits for random time before signaling that the molecule is created
    fprintf(out,"%d: O %d: molecule %d created\n", *line_num, idO, *noM);
    fflush(out);
    *line_num += 1;
    *count -= 1;
    if (*count == 0){ //if all 3 atoms completed creating of a molecule, let them finish
        sem_wait(barrier1);
        sem_post(barrier2);
    }
    sem_post(mutex2);

    sem_wait(barrier2);
    sem_post(barrier2);
    *noM += 1; //set number for the next molecule

    *NO_remaining -= 1; //decrease number of remaining oxygen atoms

    sem_post(mutex); //release mutex to allow forming of another molecule

    //if there are not enough atoms to create another molecule, release all remaining atoms from the queue
    if(*NH_remaining <= 1 || *NO_remaining < 1) {
        for(int i = 0; i < *NO_remaining; i++) {
            sem_post(oxyQueue);
        }
        for(int i = 0; i < *NH_remaining; i++){
            sem_post(hydroQueue);
        }
    }
}

/**
 * @brief Function simulating hydrogen queue
 * @param idH id of hydrogen process
 * @param TI maximum time in ms before atom joins the queue
 * @param oxygen number of oxygens in queue
 * @param hydrogen number of hydrogens in queue
 * @param line_num line number (number of action)
 * @param noM molecule id
 * @param count number of atoms in process of creating a molecule (min 0 max 3)
 * @param NH_remaining remaining number of hydrogen atoms
 * @param NO_remaining remaining number of oxygen atoms
 * @param atoms total number of created processes
 * @param commit_genocide flag for failed fork and terminating all children
 */

void hyd_queue(const int idH, long TI, int *oxygen, int *hydrogen, int *line_num, const int *noM, int *count, int *NH_remaining, const int *NO_remaining, int *atoms, const int *commit_genocide){
    srandom(getpid() * time(NULL)); //for truly random number generating

    //barrier waiting for all processes to be created before starting
    sem_wait(mutex2);
    *atoms += 1;
    sem_post(mutex2);
    if(*atoms == (*NH_remaining + *NO_remaining)){
        sem_post(queue_barrier);
    }
    sem_wait(queue_barrier);
    sem_post(queue_barrier);

    if(*commit_genocide == 1){ //if fork failed, kill all children
        exit(1);
    }

    //start of hydrogen and joining the queue
    sem_wait(mutex2);
    fprintf(out,"%d: H %d: started\n", *line_num, idH);
    fflush(out);
    *line_num += 1;
    sem_post(mutex2);
    usleep((random()%(TI+1)) * 1000); //hydrogen waits for random time before joining the queue
    sem_wait(mutex2);
    fprintf(out,"%d: H %d: going to queue\n", *line_num, idH);
    fflush(out);
    *line_num += 1;
    sem_post(mutex2);

    //if there are at least 1 atom of oxygen and 2 atoms of hydrogen in the queue, they start creating a molecule
    sem_wait(mutex);
    *hydrogen += 1;
    if (*hydrogen >=2 && *oxygen  >= 1){ //if there is enough atoms to create a molecule, release them from queue
        sem_post(hydroQueue);
        sem_post(hydroQueue);
        *hydrogen -= 2;
        sem_post(oxyQueue);
        *oxygen -= 1;
    }
    else{ //if there is not yet enough atoms in the queue needed for creating a molecule, release mutex and wait for more atoms
        sem_post(mutex);
    }

    sem_wait(hydroQueue); //queue of hydrogen atoms waiting to be released to create a molecule

    if(*NH_remaining <= 1 || *NO_remaining < 1){ //checks, whether there is enough atoms of hydrogen to create a molecule
                                                 //if not, prints out status information and process ends
        sem_wait(mutex2);
        fprintf(out,"%d: H %d: not enough O or H\n", *line_num, idH);
        fflush(out);
        *line_num += 1;
        sem_post(mutex2);
        exit(0);
    }

    //reusable barrier for creating a molecule
    sem_wait(mutex2);
    fprintf(out,"%d: H %d: creating molecule %d\n", *line_num, idH, *noM);
    fflush(out);
    *line_num += 1;
    *count += 1;
    if(*count == 3){ //wait for all 3 atoms to create the molecule
        sem_wait(barrier2);
        sem_post(barrier1);
    }
    sem_post(mutex2);

    sem_wait(barrier1);
    sem_post(barrier1);

    sem_wait(mutex2);
    fprintf(out,"%d: H %d: molecule %d created\n", *line_num, idH, *noM);
    fflush(out);
    *line_num += 1;
    *count -= 1;
    if(*count == 0){ //if all 3 atoms completed creating of a molecule, let them finish
        sem_wait(barrier1);
        sem_post(barrier2);
    }
    sem_post(mutex2);

    sem_wait(barrier2);
    sem_post(barrier2);

    *NH_remaining -= 1; //decrease number of remaining hydrogen atoms

    //if there are not enough atoms to create another molecule, release all remaining atoms from the queue
    if(*NH_remaining <= 1 || *NO_remaining < 1){
        for(int i = 0; i < *NO_remaining; i++) {
            sem_post(oxyQueue);
        }
        for(int i = 0; i < *NH_remaining; i++) {
            sem_post(hydroQueue);
        }
    }
}

/**
 * @brief Function for controlling program arguments
 * @param argc number of passed arguments
 * @param argv array of passed arguments
 * @param NO argument 1 - number of oxygen atoms
 * @param NH argument 2 - number of hydrogen atoms
 * @param TI argument 3 - maximum time in ms for atom to wait before joining the queue
 * @param TB argument 4 - maximum time in ms for oxygen to wait before signaling that the molecule was created
 * @return returns 1 if any passed argument is invalid, otherwise returns 0
 */

int check_args(int argc, char *argv[], long *NO, long *NH, long *TI, long *TB){
    //check whether correct number of arguments was passed to the program
    if (argc != 5){
        fprintf(stderr,"Error:  Wrong number of arguments.\n");
        return 1;
    }
    //variables to store non-numerical characters in program arguments
    char *o;
    char *h;
    char *i;
    char *b;

    //checks whether any of the arguments in an empty string
    if(strcmp(argv[1], "") == 0){
        fprintf(stderr, "Error: Invalid argument NO.\n");
        return 1;
    }
    else if(strcmp(argv[2], "") == 0){
        fprintf(stderr, "Error: Invalid argument NH.\n");
        return 1;
    }
    else if(strcmp(argv[3], "") == 0){
        fprintf(stderr, "Error: Invalid argument TI.\n");
        return 1;
    }
    else if(strcmp(argv[4], "") == 0){
        fprintf(stderr, "Error: Invalid argument TB.\n");
        return 1;
    }

    //getting numbers from passed arguments
    *NO = strtol((const char*) argv[1], &o, 10);
    *NH = strtol((const char*) argv[2], &h, 10);
    *TI = strtol((const char*) argv[3], &i, 10);
    *TB = strtol((const char*) argv[4], &b, 10);

    //check whether argument NO has correct value
    if (*NO <= 0){
        fprintf(stderr,"Error:  Argument NO has to be greater than zero.\n");
        return 1;
    }
    //check whether argument NH has correct value
    if (*NH <= 0){
        fprintf(stderr,"Error:  Argument NH ha to be greater than zero.\n");
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

    //variables for storing values of program arguments
    long NO = 0;
    long NH = 0;
    long TI = 0;
    long TB = 0;

    //hydrogen and oxygen id numbers
    int idO = 0;
    int idH = 0;

    //shared memory variables
    int *oxygen = NULL;
    int *hydrogen = NULL;
    int *line_num = NULL;
    int *noM = NULL;
    int *count = NULL;
    int *NH_remaining = NULL;
    int *NO_remaining = NULL;
    int *atoms = NULL;
    int *commit_genocide = NULL;

    //check for program argument errors
    int err = check_args(argc, argv, &NO, &NH, &TI, &TB);
    if(err){
        return err;
    }

    //opening output file and checking for error
    if ((out = fopen("proj2.out","w")) == NULL){
        fprintf(stderr, "Error: Couldn't open output file.\n");
        return 1;
    }

    setbuf(out, NULL); //sets file buffer to NULL for correct printing

    //initialize shared variables and check for errors
    if(create_shmem(NO, NH, &oxygen, &hydrogen, &line_num, &noM, &count, &NH_remaining, &NO_remaining, &atoms, &commit_genocide) == 1){
        //if memory mapping failed, clear all already mapped memory, print error message and return
        clear_shmem(&oxygen, &hydrogen, &line_num, &noM, &count, &NH_remaining, &NO_remaining, &atoms, &commit_genocide);
        fprintf(stderr, "Error: Shared memory mapping of variables failed.\n");
        return 1;
    }

    //initialize semaphores and check for any errors
    int sems_err = sems_init();
    if(sems_err == 1){ //if there was an error in memory mapping, clear all semaphores, print error message and return
        sems_dest();
        fprintf(stderr, "Error: Semaphore memory mapping failed.\n");
        return 1;
    }
    else if(sems_err == 2){ //if there was an error in semaphore initialization, clear all semaphores, print error message and return
        sems_dest();
        fprintf(stderr, "Error: Semaphore initialization failed.\n");
        return 1;
    }

    //loop creating NO processes of oxygen
    for (int i = 1; i <= NO; i++){
        idO++;
        pid_t id = fork();
        if(id == 0) { //child process (oxygen) goes to queue
            ox_queue(idO, TI, TB, oxygen, hydrogen, line_num, noM, count, NH_remaining, NO_remaining, atoms, commit_genocide);
            exit(0); //child dies
        }
        //error checking for failed fork()
        else if(id == -1){
            *commit_genocide = 1;
            sem_post(queue_barrier); //release children from queue, then they die
            fprintf(stderr, "Error: Creating of oxygen %d failed.\n", idO);
            while(wait(NULL) > 0); //wait for all children to die before ending the main process
            return 1;
        }
    }

    //loop creating NH processes of hydrogen
    for (int i = 1; i <= NH; i++){
        idH++;
        pid_t id = fork();
        if(id == 0) { //child process (hydrogen) goes to queue
            hyd_queue(idH, TI, oxygen, hydrogen, line_num, noM, count, NH_remaining, NO_remaining, atoms, commit_genocide);
            exit(0); //child dies
        }
        //error checking for failed fork()
        else if(id == -1){
            *commit_genocide = 1;
            sem_post(queue_barrier); //release children from queue, then they die
            fprintf(stderr, "Error: Creating of hydrogen %d failed.\n", idH);
            while(wait(NULL) > 0); //wait for all children to die before ending the main process
            return 1;
        }
    }
    //waiting for all child processes to finish
    while(wait(NULL) > 0); //wait for all children to die before ending the main process

    //closing output file
    fclose(out);
    //clearing shared variable memory
    clear_shmem(&oxygen, &hydrogen, &line_num, &noM, &count, &NH_remaining, &NO_remaining, &atoms, &commit_genocide);
    //clearing shared semaphore memory and destroying semaphores
    sems_dest();

    return 0;
}
