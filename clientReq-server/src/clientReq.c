#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h> 

#include "../../lib/include/err_lib.h"
#include "../../lib/include/server_lib.h"
#include "../../lib/include/globals.h"

// Globals for FIFOs paths
const char *path_to_server_FIFO;
const char *path_to_client_FIFO;

int main(int argc, char *argv[]) {
    char id[256], service[7];

    path_to_server_FIFO = "./FIFO/FIFOSERVER";
    path_to_client_FIFO = "./FIFO/FIFOCLIENT";

    // ========== KEY GENERATION ==========
    printf("■■■■■■■■■■■■■■■  Welcome!  ■■■■■■■■■■■■■■■\n\nServices:\n1) Stampa\n2) Salva\n3) Invia\n\nID: ");
    scanf("%255s", id);

    printf("Service: ");
    scanf("%6s", service);

    // Turns service lower_case so I don't have to check for it later
    lower_case(service);

    if(validate_service(service) == -1) {
        err_exit("<Client Request> Invalid service!");
    }

    printf("\n");

    Request_t request;

    // Assigns the values to the Request
    strcpy(request.id, id);
    strcpy(request.service, service);

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
    if(close(FIFOSERVER) == -1) {
        err_exit("<Client Request> close FIFOSERVER failed");
    }

    Response_t user_key[0];

    // Opens FIFOCLIENT/FIFOSERVER and place them into FDT
    printf("Opening FIFOCLIENT...\t\t");

    int FIFOCLIENT = open(path_to_client_FIFO, O_RDWR, S_IRUSR);

    // Checks on errors opning FIFOCLIENT/FIFOSERVER
    if(FIFOCLIENT == -1) {
        err_exit("<Client Request> open FIFOCLIENT failed");
    } else {
        printf("DONE!\n");
    }

    // Reads the resposnse from the FIFOCLIENT
    printf("Reading from FIFOCLIENT...\t");

    if(read(FIFOCLIENT, user_key, sizeof(user_key)) == -1) {
        err_exit("\n<Client Request> read from FIFOCLIENT failed"); 
    } else {
        printf("DONE!\n");
    }

    // Prints the user_key
    printf("\nKey: ");
    printf("%i\n", user_key -> user_key);

    // Checks on errors closing FIFOCLIENT file descriptor
    if(close(FIFOCLIENT) == -1) {
        err_exit("<Client Request> close FIFOCLIENT failed");        
    }

    // Checks fo errors unlinking the FIFO
    if(unlink(path_to_client_FIFO) == -1) {
        err_exit("<Client Request> unlink FIFOCLIENT failed");
    }

    return 0;
}

