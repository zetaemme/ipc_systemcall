#include "../include/sem_lib.h"
#include "../include/err_lib.h"

void semOp(int semid, unsigned short sem_num, short sem_op) {
    struct sembuf sop;

    sop.sem_num = sem_num;
    sop.sem_op = sem_op;
    sop.sem_flg = 0;

    if(semop(semid, &sop, 1) == -1) {
        errExit("semop failed");
    }
}