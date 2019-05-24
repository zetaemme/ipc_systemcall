#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../inc/errExit.h"
#include "../inc/clientReq.h"
#include "../inc/server.h"

int main (int argc, char *argv[]) {
    char id[256], service[7];

    // Path to FIFOCLIENT/FIFOSERVER location in filesystem
    char *path2ClientFIFO = "FIFOs/FIFOCLIENT";
    char *path2ServerFIFO = "FIFOs/FIFOSERVER";

    // FIFOCLIENT file descriptor
    int FIFOCLIENT;

    // FIFOSERVER file descriptor
    int FIFOSERVER;

    // Checks for error making FIFOCLIENT
    if(mkfifo(path2ClientFIFO, S_IRUSR | S_IWUSR) == -1) {
        errExit("<Client Request> mkfifo FIFOCLIENT failed");
    }

    // Opens FIFOCLIENT/FIFOSERVER and place them into FDT
    FIFOCLIENT = open(path2ClientFIFO, O_RDONLY);
    FIFOSERVER = open(path2ServerFIFO, O_WRONLY);

    // Checks on errors opning FIFOCLIENT/FIFOSERVER
    if(FIFOSERVER == -1) {
        errExit("<Client Request> open FIFOSERVER failed");
    } else if(FIFOCLIENT == -1) {
        errExit("<Client Request> open FIFOCLIENT failed");
    }

    // ========== KEY GENERATION ==========
    do {
        printf("■■■■■■■■■■■■■■■  Welcome!  ■■■■■■■■■■■■■■■\n\nServices:\n1) Stampa\n2) Salva\n3) Invia\n\nID: ");
        scanf("%255s", id);

        printf("Service: ");
        scanf("%6s", service);
    } while(validate_service(service) == -1);

    Request_t *request;

    // Assigns the values to the Request
    strcpy(request -> id, id);
    strcpy(request -> service, service);

    // Writes the request on the FIFOCLIENT
    if(write(FIFOSERVER, request, sizeof(Request_t *)) != sizeof(request)) {
        errExit("<Client Request> write on FIFOSERVER failed");
    }

    sleep(3);

    Response_t *user_key;

    // Reads the resposnse from the FIFOCLIENT
    if(read(FIFOCLIENT, user_key, sizeof(Response_t *)) == -1) {
        errExit("<Client Request> read from FIFOCLIENT failed");
    }

    // Prints the user_key
    printf("Key: ");
    print_key(user_key);
    printf("\n");
    // ========================================

    // Checks on errors closing FIFOCLIENT file descriptor
    if(close(FIFOCLIENT) == -1) {
        errExit("<Client Request> close FIFOCLIENT failed");
    }

    // Checks on errors closing FIFOSERVER file descriptor
    if(close(FIFOSERVER) == -1) {
        errExit("<Client Request> close FIFOSERVER error");
    }

    // Checks fo errors unlinking the FIFO
    if(unlink(path2ClientFIFO) == -1) {
        errExit("<Client Request> unlink FIFOCLIENT failed");
    }

    return 0;
}

int validate_service(char service[]) {
    if(strcmp(service, "Stampa") < 0 || strcmp(service, "Salva") < 0 || strcmp(service, "Invia") < 0) {
        return -1;
    }

    return 1;
}