#ifndef SERVER_LIB_H
#define SERVER_LIB_H

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./request_lib.h"
#include "./str_lib.h"
#include "./list_lib.h"

// Struct that defines server's response, using typedef for clean code 
typedef struct Response_s {
    int user_key;
} Response_t;

int generate_key(char service[], Response_t *response);
int validate_service(char service[]);

#endif