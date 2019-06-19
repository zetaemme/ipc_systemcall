#ifndef SIG_LIB_H
#define SIG_LIB_H

#include <signal.h>

#include "./err_lib.h"
#include "./globals.h"

pid_t son_process_pid;
volatile int sig_caught;

void sig_handler(int sig);

#endif