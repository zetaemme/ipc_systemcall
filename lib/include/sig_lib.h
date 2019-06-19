#ifndef SIG_LIB_H
#define SIG_LIB_H

#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "./err_lib.h"
#include "./globals.h"

const char *path_to_server_FIFO;

void server_sig_handler(int sig);
void km_sig_handler(int sig);

#endif