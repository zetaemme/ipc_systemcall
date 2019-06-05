#include "../include/str_lib.h"
#include "../include/server_lib.h"

// Slices src string from start to end and saves it into dest
void str_slice(const char *src, char *dest, size_t start, size_t end) {
    size_t j = 0;

    for (size_t i = start; i <= end; ++i) {
        dest[j++] = src[i];
    }
}

int validate_service(char service[]) {
    if(strcmp(service, "Stampa") < 0 || strcmp(service, "Salva") < 0 || strcmp(service, "Invia") < 0) {
        return -1;
    }

    return 1;
}

// Coverts *user_key to a string
char *userkey_to_string(Response_t *user_key) { 
    char numeric[5];
    
    // Converts int array into string
    for(int i = 0; i < 4; i++) {
        numeric[i] = ((user_key -> user_key_numeric) + i) + '0';
    }

    numeric[4] = '\0';

    return strcat(numeric, user_key -> user_key_service);
}