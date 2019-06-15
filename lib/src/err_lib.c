#include "../../lib/include/err_lib.h"

// Implementation of errExit
void err_exit(const char *msg) {
    perror(msg);
    exit(1);
}