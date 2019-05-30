#ifndef LIST_UTILS_H
#define LIST_UTILS_H

#include "../../clientReq-server/include/server.h"

// Nodes of the shared memory list
typedef struct Node_s {
    Data_t *value;
    struct Node_s *next;
} Node_t;

// Shared memory list
typedef struct List_s {
    Node_t *head;
} List_t;

List_t *allocate_shmem_list(Node_t *head);
void insert_list(List_t *list, Node_t *node);
void delete_from_list(List_t *list, Node_t *node);

#endif