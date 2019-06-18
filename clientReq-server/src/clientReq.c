#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h> 

#include "../../lib/include/err_lib.h"
#include "../../lib/include/server_lib.h"

int main(int argc, char *argv[]) {
    char id[256], service[7];

    // Path to FIFOCLIENT/FIFOSERVER location in filesystem
    static const char *path_to_client_FIFO = "./FIFO/FIFOCLIENT";
    static const char *path_to_server_FIFO = "./FIFO/FIFOSERVER";

    // Checks for error making FIFOCLIENT
    printf("Creating FIFOCLIENT...\t\t");

    if(mkfifo(path_to_client_FIFO, S_IRUSR | S_IWUSR) == -1) {
        err_exit("<Client Request> mkfifo FIFOCLIENT failed");
    } else {
        printf("DONE!\n");
    }

    // Opens FIFOCLIENT/FIFOSERVER and place them into FDT
    printf("Opening FIFOCLIENT...\t\t");

    int FIFOCLIENT = open(path_to_client_FIFO, O_RDWR, S_IRUSR);

    // Checks on errors opning FIFOCLIENT/FIFOSERVER
    if(FIFOCLIENT == -1) {
        err_exit("<Client Request> open FIFOCLIENT failed");
    } else {
        printf("DONE!\n");
    }

    printf("\n");

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

    printf("Opening FIFOSERVER...\t\t");

    // Opens FIFOSERVER and add it to the FDT
    int FIFOSERVER = open(path_to_server_FIFO, O_CREAT | O_WRONLY, S_IWUSR);

    // Checks if open happened
    if(FIFOSERVER == -1) {
        err_exit("\n\n<Client Request> open FIFOSERVER failed");
        
    } else {
        printf("DONE!\n");
        
    }

    // Writes the request on the FIFOCLIENT
    if(write(FIFOSERVER, &request, sizeof(Request_t *)) != sizeof(request)) {
        err_exit("<Client Request> write on FIFOSERVER failed");
        
    }

    sleep(3);

    Response_t user_key;

    // Reads the resposnse from the FIFOCLIENT
    if(read(FIFOCLIENT, &user_key, sizeof(Response_t *)) == -1) {
        err_exit("<Client Request> read from FIFOCLIENT failed");
        
    }

    // Prints the user_key
    printf("Key: ");
    print_key(&user_key);
    printf("\n");
    // ========================================

    // Checks on errors closing FIFOCLIENT file descriptor
    if(close(FIFOCLIENT) == -1) {
        err_exit("<Client Request> close FIFOCLIENT failed");
        
    }

    // Checks on errors closing FIFOSERVER file descriptor
    if(close(FIFOSERVER) == -1) {
        err_exit("<Client Request> close FIFOSERVER error");
    }

    // Checks fo errors unlinking the FIFO
    if(unlink(path_to_client_FIFO) == -1) {
        err_exit("<Client Request> unlink FIFOCLIENT failed");
    }

    return 0;
}

