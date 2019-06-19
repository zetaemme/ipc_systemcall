#include "../include/list_lib.h"

// Instert a new node in the data list
void insert_list(List_t *list, char id[], Response_t *user_key) {
    // Node to insert and iteration node
    Node_t *current = list -> head;
    Node_t *new_node = NULL;

    // Inits the node to insert
    strcpy(new_node -> value -> id, id);
    new_node -> value -> user_key = user_key;
    gettimeofday(&(new_node -> value) -> timestamp, NULL);
    new_node -> value -> has_been_used = 0;

    // Checks for an empty list
    if(current == NULL){
        list -> head = new_node;
        new_node -> next = NULL;
    } else {
        // Goes to the last node
        while(current -> next != NULL) {
            current = current -> next;
        }

        // Assigns the new node on the tail
        current -> next = new_node;
        current -> next -> next = NULL;
    }
}

//Delete a node from the data list
void delete_from_list(List_t *list, Node_t *node) {
    // Iteration nodes
    Node_t *current = list -> head;
    Node_t *previous = NULL;
    
    // If we have a 1 element list and that element is the designed element
    if(current -> next == NULL && check_eq_data(current -> value, node -> value)) {
        current = NULL;
    } else {
        // For each node in the list
        while(current -> next != NULL) {
            // Checks if it is the designed node
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