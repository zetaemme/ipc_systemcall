#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../inc/errExit.h"
#include "../inc/clientReq.h"

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

    // ========== USER INPUT SECTION ==========
    printf("■■■■■■■■■■■■■■■  Welcome!  ■■■■■■■■■■■■■■■\n\nServices:\n1) Stampa\n2) Salva\n3) Invia\n\nID: ");
    scanf("%255s", id);

    printf("Service: ");
    scanf("%6s", service);
    // ========================================

    // Placeholder stampa server-key

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
