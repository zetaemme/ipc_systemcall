#ifndef SERVER_H
#define SERVER_H

#define STRMAX 3

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Struct that defines server's response, using typedef for clean code 
typedef struct Response_s {
    int user_key_numeric[4];
    char user_key_service[4];
} Response_t;

// Generates a user_key
Response_t generate_key(char service[]) {
    Response_t result;

    // Init random seed
    srand(time(NULL));

    // Generates 4 random integers
    for(int i = 0; i < 4; i++) {
        result.user_key_numeric[i] = rand() % 10;
    }

    // Checks for service type and assigns service part of user_key
    if(strcmp(service, "Stampa") >= 0) {
        strcpy(result.user_key_service, "prt");
    } else if(strcmp(service, "Salva") >= 0) {
        strcpy(result.user_key_service, "sav");
    } else {
        strcpy(result.user_key_service, "snd");
    }

    return result;
}

// Function to print user_key (DEBUG)
void __print_key(Response_t *response) {
    for(int i = 0; i < 4; i++) {
        printf("%i", response -> user_key_numeric[i]);
    }

    printf("%s\n", response -> user_key_service);
}

#endif