#ifndef LIST_LIB_H
#define LIST_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "./err_lib.h"

// Nodes of the shared memory list
typedef struct Node_s {
    char id[256];
    int user_key;
    time_t timeval;
    int has_been_used;
} Node_t;

void write_in_shared_memory(Node_t *shm_list, char id[], int user_key);
int get_timestamp();
int check_eq_data(Node_t *data1, Node_t *data2);
void delete_from_shared_memory(Node_t *node);
void print_node(Node_t *node);
bool has_been_used(Node_t *node);

#endif