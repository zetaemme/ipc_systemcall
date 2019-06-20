#ifndef REQUEST_LIB_H
#define REQUEST_LIB_H

#include <sys/types.h>

typedef struct Request_s {
    char id[256];
    char service[7];
} Request_t;

#endif