#include "../include/errExit.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

// Implementation of errExit
void errExit(const char *msg) {
    perror(msg);
    exit(1);
}