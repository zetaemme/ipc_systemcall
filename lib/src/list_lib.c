#include "../include/list_lib.h"

// Instert a new node in the data list
void insert_list(List_t *list, char id[], int user_key) {
    // Node to insert and iteration node
    Node_t *current = list -> head;
    Node_t new_node;

    // Inits the node to insert
    strcpy(new_node.id, id);
    new_node.user_key = user_key;
    new_node.timeval = get_timestamp();
    new_node.has_been_used = 0;

    print_node(&new_node);
    
    // Checks for an empty list
    if(current == NULL){
        list -> head = &new_node;
        new_node.next = NULL;
    } else {
        // Goes to the last node
        while(current -> next != NULL) {
            current = current -> next;
        }

        // Assigns the new node on the tail
        current -> next = &new_node;
        current -> next -> next = NULL;
    }
}

// Get seconds from the start of the day
int get_timestamp(){
    time_t rawtime;     
    struct tm *info;

    time(&rawtime);
    info = localtime(&rawtime);   

    return (info->tm_hour * 3600) + (info->tm_min * 60) + info->tm_sec ;
}

// Compare two different data
int check_eq_data(Node_t *data1, Node_t *data2) {
    if(strcmp(data1 -> id, data2 -> id) == 0 && 
        data1 -> user_key == data2 -> user_key && 
        data1 -> timeval == data2 -> timeval
      ){

        return 1;
    }

    return 0;
}

//Delete a node from the data list
void delete_from_list(List_t *list, Node_t *node) {
    // Iteration nodes
    Node_t *current = list -> head;
    Node_t *previous = NULL;
    
    // If we have a 1 element list and that element is the designed element
    if(current -> next == NULL && check_eq_data(current, node)) {
        current = NULL;
    } else {
        // For each node in the list
        while(current -> next != NULL) {
            // Checks if it is the designed node
            if(check_eq_data(current, node)) {
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

void print_node(Node_t *node) {
    printf("Node:\n");
    printf("\n%s\n", node -> id);
    printf("%i\n", node -> user_key);
    printf("%li\n", node -> timeval);
    printf("%i\n", node -> has_been_used);
}