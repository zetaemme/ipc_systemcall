#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../inc/errExit.h"
#include "../inc/server.h"

int main (int argc, char *argv[]) {
    // Path to FIFOSERVER location in filesystem
    char *path2ServerFIFO;

    // FIFOSERVER file desctiptor
    int FIFOSERVER;

    // Checks on error making FIFOSERVER
    if(mkfifo(path2ServerFIFO, S_IWUSR) == -1) {
        errExit("mkfifo FIFOSERVER falied");
    }

    // Opens FIFOSERVER and places it in FDT
    FIFOSERVER = open(path2ServerFIFO, O_RDWR);

    // ========== OPERATION SECTION ==========
    // Generate user_key
    Response_t user_key = generate_key(/* Servzizo richiesto letto da FIFOCLIENT */);
    // =======================================

    // Checks on errors closing FIFOSERVER file descriptor
    if(close(FIFOSERVER) == -1) {
        errExit("close FIFOSERVER failed");
    }

    // Checks fo errors unlinking the FIFO
    if(unlink(path2ServerFIFO) == -1) {
        errExit("unlink FIFOSERVER failed");
    }

    return 0;
}
