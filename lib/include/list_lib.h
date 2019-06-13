#ifndef LIST_LIB_H
#define LIST_LIB_H

#include <stdlib.h>

#include "./server_lib.h"

// Nodes of the shared memory list
typedef struct Node_s {
    Data_t *value;
    struct Node_s *next;
} Node_t;

// Shared memory list
typedef struct List_s {
    Node_t *head;
} List_t;

void insert_list(List_t *list, char id[], Response_t *user_key);
void delete_from_list(List_t *list, Node_t *node);

#endif