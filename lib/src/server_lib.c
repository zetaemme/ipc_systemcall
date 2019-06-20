#include "../include/server_lib.h"

// Generates a user_key
int generate_key(char service[], Response_t *response) {
    // Init random seed
    srand(time(NULL));

    /*  Checks for service type and assigns service part of user_key
        First digit is:
            1) Service -> Stampa
            2) Service -> Salva
            3) Service -> Invia
    */
    if(strcmp(service, "stampa") == 0) {
        response -> user_key = 10000 + rand() % 9000 + 1000;
        return 1;
    } else if(strcmp(service, "salva") == 0) {
        response -> user_key = 20000 + rand() % 9000 + 1000;
        return 1;
    } else {
        response -> user_key = 30000 + rand() % 9000 + 1000;
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