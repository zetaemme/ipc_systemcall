#ifndef CLIENTREQ_H
#define CLIENTREQ_H

#include <string.h>

typedef struct Request_s {
    char id[256];
    char service[7];
} Request_t;

int validate_service(char service[]) {
    if(strcmp(service, "Stampa") < 0 || strcmp(service, "Salva") < 0 || strcmp(service, "Invia") < 0) {
        return -1;
    }

    return 1;
}

#endif