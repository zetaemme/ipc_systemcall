#ifndef SERVER_LIB_H
#define SERVER_LIB_H

#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./request_lib.h"
#include "./str_lib.h"

// Struct that defines server's response, using typedef for clean code 
typedef struct Response_s {
    int user_key_numeric[4];
    char user_key_service[4];
} Response_t;

// Struct that defines user data in the shared memory segment
typedef struct Data_s {
    char id[256];
    Response_t * user_key;
    struct timeval timestamp;
} Data_t;

void generate_key(Request_t *request, Response_t *response);
void print_key(Response_t *response);
int check_five_min_diff(struct timeval *current, struct timeval *data_timestamp);
int check_eq_data(Data_t *data1, Data_t *data2);
int validate_service(char service[]);
char *userkey_to_string(Response_t *user_key);

#endif