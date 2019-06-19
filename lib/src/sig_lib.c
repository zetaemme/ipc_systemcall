#include "../include/sig_lib.h"

void sig_handler(int sig) {
    sig_caught = 1;

    if(kill(son_process_pid, SIGTERM)) {
        err_exit("<Signal Handler> kill failed");
    }   
}