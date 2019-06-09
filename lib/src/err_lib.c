#include "../../lib/include/err_lib.h"

// Implementation of errExit
void errExit(const char *msg) {
    perror(msg);
    exit(1);
}