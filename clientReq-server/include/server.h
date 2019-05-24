#ifndef SERVER_H
#define SERVER_H

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

#include "./clientReq.h"

// Struct that defines server's response, using typedef for clean code 
typedef struct Response_s {
    int user_key_numeric[4];
    char user_key_service[4];
} Response_t;

// Struct that defines user data in the shared memory segment
typedef struct Data_s {
    char id[256];
    Response_t user_key;
    struct timeval timestamp;
} Data_t;

void generate_key(Request_t *request, Response_t *response);
void print_key(Response_t *response);
void sigHandler(int sig);
void get_timestamp(Data_t *user_data);
void on_alarm(int sig);

#endif