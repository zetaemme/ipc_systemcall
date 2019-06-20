#include "../include/server_lib.h"

// Generates a user_key
void generate_key(Request_t *request, Response_t *response) {
    // Init random seed
    srand(time(NULL));

    /*  Checks for service type and assigns service part of user_key
        First digit is:
            1) Service -> Stampa
            2) Service -> Salva
            3) Service -> Invia
    */
    if(strcmp(request -> service, "stampa") == 0) {
        response -> user_key = 10000 + rand() % 9000 + 1000;
    } else if(strcmp(request -> service, "salva") == 0) {
        response -> user_key = 20000 + rand() % 9000 + 1000;
    } else {
        response -> user_key = 30000 + rand() % 9000 + 1000;
    }
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
        data1 -> user_key -> user_key == data2 -> user_key -> user_key && 
        data1 -> timestamp.tv_sec == data2 -> timestamp.tv_sec
      ){

        return 1;
    }

    return 0;
}

// Checks if the serivice is valid
int validate_service(char service[]) {
    if(strcmp(service, "stampa") == 0 || strcmp(service, "salva") == 0 || strcmp(service, "invia") == 0) {
        return 1;
    }

    return -1;
}