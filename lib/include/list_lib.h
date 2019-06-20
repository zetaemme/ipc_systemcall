#ifndef LIST_LIB_H
#define LIST_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Nodes of the shared memory list
typedef struct Node_s {
    char id[256];
    int user_key;
    time_t timeval;
    int has_been_used;
    struct Node_s *next;
} Node_t;

// Shared memory list
typedef struct List_s {
    Node_t *head;
} List_t;

void insert_list(List_t *list, char id[], int user_key);
int get_timestamp();
int check_eq_data(Node_t *data1, Node_t *data2);
void delete_from_list(List_t *list, Node_t *node);
void print_node(Node_t *node);

#endif