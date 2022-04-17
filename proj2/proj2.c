#include <stdio.h>
#include <stdlib.h>
//#include <fcntl.h>
//#include <sys/stat.h>
//#include <limits.h>
//#include <semaphore.h>
//#include <sys/mman.h>
//#include <sys/wait.h>
//#icnlude <unistd.h>
//#include <string.h>


int error(int argc, char *argv[], long *NO, long *NH, long *TI, long *TB){
    //check whether correct number of arguments was passed
    if (argc != 5){
        fprintf(stderr,"Error:  Wrong number of arguments.");
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
        fprintf(stderr,"Error:  Argument NO cannot be negative.");
        return 1;
    }
    //check whether argument NH has correct value
    if (*NH < 0){
        fprintf(stderr,"Error:  Argument NH cannot be negative.");
        return 1;
    }
    //check whether argument TI has correct value
    if (*TI < 0 || *TI > 1000){
        fprintf(stderr,"Error:  Argument TI has to belong to interval <0;1000>.");
        return 1;
    }
    //check whether argument TB has correct value
    if (*TB < 0 || *TB > 1000){
        fprintf(stderr,"Error:  Argument TB has to belong to interval <0;1000>.");
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

    int err = error(argc, argv, &NO, &NH, &TI, &TB);
    if(err){
        return err;
    }

    printf("NO is: %ld\n", NO);
    printf("NH is: %ld\n", NH);
    printf("TI is: %ld\n", TI);
    printf("TB is: %ld\n", TB);



    return 0;
}