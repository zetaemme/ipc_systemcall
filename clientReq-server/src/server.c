#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../inc/errExit.h"
#include "../inc/server.h"
#include "../inc/clientReq.h"

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

    // Signal set
    sigset_t noSIGTERMSet;

    // Fill the signal set
    sigfillset(&noSIGTERMSet);

    // Deletes SIGTERM from mySet
    sigdelset(&noSIGTERMSet, SIGTERM);

    // Blocks all signals but SIGTERM
    sigprocmask(SIG_SETMASK, &noSIGTERMSet, NULL);

    // ========== OPERATION SECTION ==========
    // sigHandler as handler for SIGTERM
    if(signal(SIGTERM, sigHandler) == SIG_ERR) {
        errExit("SIGTERM caught. Closing!");
    }

    // Request read from FIFO
    Request_t *request;
    
    // Reads the request from FIFOSERVER
    if(read(FIFOSERVER, request, sizeof(Request_t *)) == -1) {
        errExit("read from FIFOSERVER failed");
    }

    // Response containing the user key
    Response_t *user_key;

    // Generate user_key
    if(strcmp(request -> service, "Stampa") >= 0 || strcmp(request -> service, "Salva") >= 0 || strcmp(request -> service, "Invia") >= 0) {
        user_key = generate_key(request -> service);
    }

    if(user_key != NULL) {
        // Writes the response on the FIFO
        if(write(FIFOSERVER, user_key, sizeof(Response_t *)) == 0) {
            errExit("write on FIFOSERVER failed");
        }
    }
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
