#include "../include/server_lib.h"

// Generates a user_key
void generate_key(Request_t *request, Response_t *response) {
    // Init random seed
    srand(time(NULL));

    // Generates 4 random integers
    for(int i = 0; i < 4; i++) {
        response -> user_key_numeric[i] = rand() % 10;
    }

    // Checks for service type and assigns service part of user_key
    if(strcmp(request -> service, "Stampa") == 0) {
        strcpy(response -> user_key_service, "prt");
    } else if(strcmp(request -> service, "Salva") == 0) {
        strcpy(response -> user_key_service, "sav");
    } else {
        strcpy(response -> user_key_service, "snd");
    }
}

// Function to print user_key
void print_key(Response_t *response) {
    for(int i = 0; i < 4; i++) {
        printf("%i", response -> user_key_numeric[i]);
    }

    printf("%s\n", response -> user_key_service);
}

// Checks if five minutes difference sussist between 2 timestamps
int check_five_min_diff(struct timeval *current, struct timeval *data_timestamp) {
    if((current -> tv_sec) - (data_timestamp -> tv_sec) >= 300) {
        return 1;
    }

    return 0;
}

// Compare two different data
int check_eq_data(Data_t *data1, Data_t *data2) {
    if(strcmp(data1 -> id, data2 -> id) == 0 && 
        strcmp(userkey_to_string(data1 -> user_key), userkey_to_string(data2 -> user_key)) == 0 && 
        data1 -> timestamp.tv_sec == data2 -> timestamp.tv_sec
      ){

        return 1;
    }

    return 0;
}