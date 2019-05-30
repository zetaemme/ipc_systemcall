#include "../../clientReq-server/include/server.h"
#include "../include/list_utils.h"

// Allocates the shared memory linked list
List_t *allocate_shmem_list(Node_t *head) {
    List_t *new_list = (List_t *) malloc(sizeof(List_t *));
    new_list -> head = head;

    return new_list;
}

// Instert a new node in the data list
void insert_list(List_t *list, Node_t *node) {
    Node_t * current = list -> head;

    if(current == NULL){
        list -> head = node;
    } else {
        while(current -> next != NULL) {
            current = current -> next;
        }

        current -> next = node;
        current -> next -> value = node -> value;
        current -> next -> next = NULL;
    }
}

//Delete a node from the data list
void delete_from_list(List_t *list, Node_t *node) {
    Node_t *current = list -> head;
    Node_t *previous = NULL;
    
    if(current -> next == NULL && check_eq_data(current -> value, node -> value)) {
        current == NULL;
    } else {
        while(current -> next != NULL) {
            if(check_eq_data(current -> value, node -> value)) {
                if(current == list -> head) {
                    list -> head = current -> next;
                    current = current -> next;
                } else {
                    previous -> next = current -> next;
                    current = previous -> next;
                }
            } else {
                current = current -> next;
            }
        }
    }
}