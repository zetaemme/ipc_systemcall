#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/ipc.h> 

#include "../../lib/include/err_lib.h"
#include "../../lib/include/server_lib.h"
#include "../../lib/include/globals.h"

// Globals for FIFOs paths
const char *path_to_server_FIFO;
const char *path_to_client_FIFO;

int main(int argc, char *argv[]) {
    Request_t request;

    path_to_server_FIFO = "./FIFO/FIFOSERVER";
    path_to_client_FIFO = "./FIFO/FIFOCLIENT";

    // ========== KEY GENERATION ==========
    printf("■■■■■■■■■■■■■■■  Welcome!  ■■■■■■■■■■■■■■■\n\nServices:\n1) Stampa\n2) Salva\n3) Invia\n\nID: ");
    scanf("%255s", request.id);

    printf("Service: ");
    scanf("%6s", request.service);

    // Turns service lower_case so I don't have to check for it later
    lower_case(request.service);

    if(validate_service(request.service) == -1) {
        err_exit("<Client Request> Invalid service!");
    }

    printf("\n");

    // Creates FIFOCLIENT
    printf("Creating FIFOCLIENT...\t\t");

    // Checks for error making FIFOCLIENT
    if(mkfifo(path_to_client_FIFO, S_IRUSR | S_IWUSR) == -1) {
        err_exit("<Client Request> mkfifo FIFOCLIENT failed");
    } else {
        printf("DONE!\n");
    }

    printf("Opening FIFOSERVER...\t\t");

    // FIFOSERVER file descriptor
    int FIFOSERVER = open(path_to_server_FIFO, O_WRONLY);

    // Checks if open happened
    if(FIFOSERVER == -1) {
        err_exit("\n\n<Client Request> open FIFOSERVER failed");       
    } else {
        printf("DONE!\n");
    }

    // Writes the request on the FIFOSERVER
    printf("Writing on FIFOSERVER...\t");

    // Write buffer
    Request_t wB[] = {request};

    if(write(FIFOSERVER, wB, sizeof(wB)) == -1) {
        err_exit("\n<Client Request> write on FIFOSERVER failed");       
    } else {
        printf("DONE!\n");
    }

    // Closes FIFOSERVER file descriptor
    close(FIFOSERVER);

    // Opens FIFOCLIENT/FIFOSERVER and place them into FDT
    printf("Opening FIFOCLIENT...\t\t");

    int FIFOCLIENT = open(path_to_client_FIFO, O_RDONLY);

    // Checks on errors opning FIFOCLIENT/FIFOSERVER
    if(FIFOCLIENT == -1) {
        err_exit("<Client Request> open FIFOCLIENT failed");
    } else {
        printf("DONE!\n");
    }

    Response_t bR[1];

    // Reads the resposnse from the FIFOCLIENT
    printf("Reading from FIFOCLIENT...\t");

    if(read(FIFOCLIENT, bR, sizeof(bR)) == -1) {
        err_exit("\n<Client Request> read from FIFOCLIENT failed"); 
    } else {
        printf("DONE!\n");
    }

    int key = bR[0].user_key;

    // Prints the user_key
    printf("\nUser Key: %i\n", key);

    // Checks fo errors unlinking the FIFO
    if(unlink(path_to_client_FIFO) == -1) {
        err_exit("<Client Request> unlink FIFOCLIENT failed");
    }

    return 0;
}

