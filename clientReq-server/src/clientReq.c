#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h> 

#include "../../lib/include/err_lib.h"
#include "../../lib/include/str_lib.h"
#include "../../lib/include/request_lib.h"
#include "../../lib/include/server_lib.h"

int main(int argc, char *argv[]) {
    char id[256], service[7];

    // Path to FIFOCLIENT/FIFOSERVER location in filesystem
    static const char *path2ClientFIFO = "./FIFO/FIFOCLIENT";
    static const char *path2ServerFIFO = "./FIFO/FIFOSERVER";

    // Checks for error making FIFOCLIENT
    printf("Creating FIFOCLIENT...\t\t");

    if(mkfifo(path2ClientFIFO, S_IRUSR | S_IWUSR) == -1) {
        errExit("<Client Request> mkfifo FIFOCLIENT failed");
    } else {
        printf("DONE!\n");
    }

    // Opens FIFOCLIENT/FIFOSERVER and place them into FDT
    printf("Opening FIFOCLIENT...\t\t");

    int FIFOCLIENT = open(path2ClientFIFO, O_RDWR, S_IRUSR);

    // Checks on errors opning FIFOCLIENT/FIFOSERVER
    if(FIFOCLIENT == -1) {
        errExit("<Client Request> open FIFOCLIENT failed");
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

    Request_t *request = NULL;

    // Assigns the values to the Request
    strcpy(request -> id, id);
    strcpy(request -> service, service);

    printf("Opening FIFOSERVER...\t\t");

    // Opens FIFOSERVER and add it to the FDT
    int FIFOSERVER = open(path2ServerFIFO, O_CREAT | O_WRONLY, S_IWUSR);

    // Checks if open happened
    if(FIFOSERVER == -1) {
        errExit("\n\n<Client Request> open FIFOSERVER failed");
        
    } else {
        printf("DONE!\n");
        
    }

    // Writes the request on the FIFOCLIENT
    if(write(FIFOSERVER, request, sizeof(Request_t *)) != sizeof(request)) {
        errExit("<Client Request> write on FIFOSERVER failed");
        
    }

    sleep(3);

    Response_t *user_key = NULL;

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

