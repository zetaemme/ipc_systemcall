#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../../lib/include/err_lib.h"
#include "../../lib/include/server_lib.h"
#include "../../lib/include/list_lib.h"
#include "../../lib/include/request_lib.h"
#include "../../lib/include/sig_lib.h"
#include "../../lib/include/sem_lib.h"

int main (int argc, char *argv[]) {
    // Path to the fifo files
    static const char *path2ClientFIFO = "./FIFO/FIFOCLIENT";
    static const char *path2ServerFIFO = "./FIFO/FIFOSERVER";

    // Checks on error making FIFOSERVER
    printf("Creating FIFOSERVER...\t\t");

    if(mkfifo(path2ServerFIFO, S_IWUSR | S_IRUSR) == -1) {
        printf("\033[1;31m");
        errExit("<Server> mkfifo FIFOSERVER failed");
        printf("\033[0m");
    } else {
        printf("\033[1;32m");
        printf("DONE!\n");
        printf("\033[0m");
    }    

    // Opens FIFOSERVER/FIFOCLIENT and place them in FDT
    printf("Opening FIFOSERVER...\t\t");

    int FIFOSERVER = open(path2ServerFIFO, O_CREAT | O_RDWR, S_IWUSR);

    // Checks on errors opning FIFOCLIENT/FIFOSERVER
    if(FIFOSERVER == -1) {
        printf("\033[1;31m");
        errExit("<Client Request> open FIFOSERVER failed");
        printf("\033[0m");
    } else {
        printf("\033[1;32m");
        printf("DONE!\n");
        printf("\033[0m");
    }

    printf("Generating shm key...\t\t");

    // Creates the Shared Memory key
    key_t shmKey = ftok("src/server.c", 's');

    // Checks if ftok succesfully created a key
    if(shmKey == -1) {
        printf("\033[1;31m");
        errExit("\n\n<Server> shared memory ftok failed");
        printf("\033[0m");
    } else {
        printf("\033[1;32m");
        printf("DONE!\n");
        printf("\033[0m");
    }

    printf("Getting Shared Memory...\t");

    // Creates the shared memory
    int shmid = shmget(shmKey, sizeof(Node_t *) * 100, IPC_CREAT | S_IRUSR | S_IWUSR);
  
    // Checks if the shared memory was successfully created
    if(shmid == -1) {
        printf("\033[1;31m");
        errExit("\n\n<Server> shmget failed");
        printf("\033[0m");
    } else {
        printf("\033[1;32m");
        printf("DONE!\n");
        printf("\033[0m");
    }

    // Create the semaphore set
    // int semid = semget(shmKey, 1, IPC_CREAT | S_IRUSR | S_IWUSR);

    printf("Generating SigSet...\t\t");

    // Signal set
    sigset_t noSIGTERMSet;

    // Fill the signal set
    sigfillset(&noSIGTERMSet);

    // Deletes SIGTERM from mySet
    sigdelset(&noSIGTERMSet, SIGTERM);

    // Blocks all signals but SIGTERM
    sigprocmask(SIG_SETMASK, &noSIGTERMSet, NULL);

    // ========== SERVER OPERATION SECTION ==========
    // sigHandler as handler for SIGTERM
    if(signal(SIGTERM, sigHandler) == SIG_ERR) {
        printf("\033[1;31m");
        errExit("<Server> signal failed");
        printf("\033[0m");
    } else {
        printf("\033[1;32m");
        printf("DONE!\n");
        printf("\033[0m");
    }

    // Request read from FIFO
    Request_t *request = NULL;
    
    printf("Reading from FIFOSERVER...\t\t");

    // QUI SI BLOCCA PERCHE' NON HA FIFOCLIENT

    // Reads the request from FIFOSERVER
    if(read(FIFOSERVER, request, sizeof(Request_t *)) == -1) {
        printf("\033[1;31m");
        errExit("<Server> read from FIFOSERVER failed");
        printf("\033[0m");
    } else {
        printf("\033[1;32m");
        printf("DONE!\n");
        printf("\033[0m");
    }

    // Response containing the user key
    Response_t *user_key = NULL;

    // Generate user_key
    if(strcmp(request -> service, "Stampa") >= 0 || strcmp(request -> service, "Salva") >= 0 || strcmp(request -> service, "Invia") >= 0) {
        generate_key(request, user_key);
    }

    printf("Forking KeyManager...\t\t");

    // Creates the child process KeyManager
    pid_t key_manager = fork();

    // ================= KEYMANAGER ===============
    if(key_manager == 0) {

        printf("Key manager is running...\n\n");

        // Attach the shared memory segment
        List_t *attached_shm_list = (List_t *) shmat(shmid, NULL, 0);

        if(attached_shm_list == (List_t *) -1) {
            printf("\033[1;31m");
            errExit("<KeyMamager> shmat failed");
            printf("\033[0m");
        }

        // Now-time value
        struct timeval current_time;

        // Current node of the list
        Node_t *current_node = attached_shm_list -> head;

        // This loop executes each 30 seconds
        while(1) {
            // Gets the current time
            gettimeofday(&current_time, NULL);

            // FRA TE SPARO

            while(current_node -> next != NULL) {
                if(check_five_min_diff(&current_time, &(current_node -> value) -> timestamp)) {
                    delete_from_list(attached_shm_list, current_node);

                    current_node = current_node -> next;
                } else {
                    current_node = current_node -> next;
                }
            }

            // FA NA CANNA

            // Sleep for 30 seconds
            sleep(30);

            // Reset the current node to the head of the list
            current_node = attached_shm_list -> head;

            // TODO Catch di SIGTERM per uscire dal programma
            // =======================================
            // Detach this process from the Shared Memory
            if(shmdt(attached_shm_list) == -1) {
                printf("\033[1;31m");
                errExit("<KeyManager> shmdt failed");
                printf("\033[0m");
            }
            // =======================================
        }
    }
    // =============================================

    // Attach the server to the Shared Memory
    List_t *attached_shm_list = (List_t *) shmat(shmid, NULL, 0);

    // FIFOCLIENT file descriptor
    int FIFOCLIENT;

    if(user_key != NULL) {
        if(attached_shm_list == (List_t *) -1) {
            printf("\033[1;31m");
            errExit("<Server> shmat failed");
            printf("\033[0m");
        }

        // Inserts the new node in the shared memory
        insert_list(attached_shm_list, request -> id, user_key);

        // Opens FIFOCLIENT and add it to the FDT
        FIFOCLIENT = open(path2ClientFIFO, O_RDWR, S_IRUSR);

        // Checks if open happened
        if(FIFOCLIENT == -1) {
            printf("\033[1;31m");
            errExit("<Server> open FIFOCLIENT failed");
            printf("\033[0m");
        }

        // Writes the response on the FIFO
        if(write(FIFOCLIENT, user_key, sizeof(Response_t *)) == 0) {
            printf("\033[1;31m");
            errExit("<Server> write on FIFOSERVER failed");
            printf("\033[0m");
        }
    } else {
        printf("\033[1;31m");
        errExit("<Server> NULL user key");
        printf("\033[0m");
    }

    // Waits for SIGTERM
    // TODO Catch di SIGTERM per uscire dal programma
    // =======================================
    // Detach this process from the Shared Memory
    if(shmdt(attached_shm_list) == -1) {
        printf("\033[1;31m");
        errExit("<Server> shmdt failed");
        printf("\033[0m");
    }

    // Deletes the shared memory
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        printf("\033[1;31m");
        errExit("<Server> shmctl IPC_RMID failed");
        printf("\033[0m");
    }

    // Checks on errors closing FIFOSERVER file descriptor
    if(close(FIFOSERVER) == -1) {
        printf("\033[1;31m");
        errExit("<Server> close FIFOSERVER failed");
        printf("\033[0m");
    }

    // Checks on errors closing FIFOCLIENT file descriptor
    if(close(FIFOCLIENT) == -1) {
        printf("\033[1;31m");
        errExit("<Server> close FIFOCLIENT failed");
        printf("\033[0m");
    }

    // Checks fo errors unlinking the FIFO
    if(unlink(path2ServerFIFO) == -1) {
        printf("\033[1;31m");
        errExit("<Server> unlink FIFOSERVER failed");
        printf("\033[0m");
    }
    // =======================================

    return 0;
}