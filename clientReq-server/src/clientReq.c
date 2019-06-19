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
    do {
        printf("■■■■■■■■■■■■■■■  Welcome!  ■■■■■■■■■■■■■■■\n\nServices:\n1) Stampa\n2) Salva\n3) Invia\n\nID: ");
        scanf("%255s", id);

        printf("Service: ");
        scanf("%6s", service);
    } while(validate_service(service) == -1);

    Request_t request;

    // Turns service lower_case so I don't have to check for it later
    lower_case(service);

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

    // Opens FIFOSERVER and add it to the FDT
    int FIFOSERVER = open(path_to_server_FIFO, O_WRONLY);

    // Checks if open happened
    if(FIFOSERVER == -1) {
        err_exit("\n\n<Client Request> open FIFOSERVER failed");       
    } else {
        printf("DONE!\n");
    }

    // Writes the request on the FIFOSERVER
    printf("Writing on FIFOSERVER...\t");

    if(write(FIFOSERVER, &request, sizeof(request)) == -1) {
        err_exit("\n<Client Request> write on FIFOSERVER failed");       
    } else {
        printf("DONE!\n");
    }

    // Closes FIFOSERVER file descriptor
    if(close(FIFOSERVER) == -1) {
        err_exit("<Client Request> close FIFOSERVER failed");
    }

    Response_t user_key;

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
    printf("Reading from FIFOCLIENT...\t\t");

    if(read(FIFOCLIENT, &user_key, sizeof(Response_t)) == -1) {
        err_exit("\n<Client Request> read from FIFOCLIENT failed"); 
    } else {
        printf("DONE!\n");
    }

    // Prints the user_key
    printf("Key: ");
    print_key(&user_key);
    printf("\n");

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

