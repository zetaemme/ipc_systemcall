#ifndef SEM_LIB_H
#define SEM_LIB_H

#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

#endif