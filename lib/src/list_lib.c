#include "../include/list_lib.h"

// Instert a new node in the data list
void write_in_shared_memory(Node_t *shm_list, char id[], int user_key) {
    Node_t *current = shm_list;
    int i;

    for(i = 0; i < 100; i++) {
        if((strcmp(current -> id, "") != 0 && current -> has_been_used == 1) || strcmp(current -> id, "") == 0) {
            break;
        }

        current++;
    }

    if(i == 99) {
        printf("\n<Shared Memory> full memory");
    }

    strcpy(current -> id, id);
    current -> user_key = user_key;
    current -> timeval = get_timestamp();
    current -> has_been_used = 0;
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
void delete_from_shared_memory(Node_t *node) {
    node -> has_been_used = 0;
}

void print_node(Node_t *node) {
    printf("ID: %s\t\t", node -> id);
    printf("KEY: %i\t\t", node -> user_key);
    printf("TIME: %li\t\t", node -> timeval);
    printf("USED: %i\n", node -> has_been_used);
}

bool has_been_used(Node_t *node) {
    return node -> has_been_used == 0;
}