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

    // Path to FIFOCLIENT location in filesystem
    char *path2ClientFIFO;

    // FIFOCLIENT file descriptor
    int FIFOCLIENT;

    // Checks for error making FIFOCLIENT
    if(mkfifo(path2ClientFIFO, S_IRUSR) == -1) {
        errExit("mkfifo FIFOCLIENT failed");
    }

    // Opens FIFOCLIENT and places it into FDT
    FIFOCLIENT = open(path2ClientFIFO, O_RDONLY);

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
    if(write(FIFOCLIENT, request, sizeof(Request_t *)) != sizeof(request)) {
        errExit("write on FIFOCLIENT failed");
    }

    sleep(3);

    Response_t *user_key;

    // Reads the resposnse from the FIFOCLIENT
    if(read(FIFOCLIENT, user_key, sizeof(Response_t *)) == -1) {
        errExit("read from FIFOCLIENT failed");
    }

    // Prints the user_key
    printf("Key: ");
    print_key(user_key);
    printf("\n");
    // ========================================

    // Checks on errors closing FIFOSERVER file descriptor
    if(close(FIFOCLIENT) == -1) {
        errExit("close FIFOCLIENT failed");
    }

    // Checks fo errors unlinking the FIFO
    if(unlink(path2ClientFIFO) == -1) {
        errExit("unlink FIFOCLIENT failed");
    }

    return 0;
}
