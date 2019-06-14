#ifndef SIG_LIB_H
#define SIG_LIB_H

#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>

#include "./list_lib.h"
#include "./err_lib.h"

void server_sig_handler(int sig, List_t* shared_memory_list, int FIFOSERVER, int FIFOCLIENT, int shmid);
void km_sig_handler(int sig, List_t* shared_memory_list);

#endif
