#ifndef SEM_LIB_H
#define SEM_LIB_H

#include <sys/sem.h>

#include "./err_lib.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void semOp(int sem_id, unsigned short sem_num, short sem_op);

#endif