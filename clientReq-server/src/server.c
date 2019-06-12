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
    printf("Creating FIFOSERVER...\n");

    if(mkfifo(path2ServerFIFO, S_IWUSR | S_IRUSR) == -1) {
        errExit("<Server> mkfifo FIFOSERVER failed");
    }

    // Opens FIFOSERVER/FIFOCLIENT and place them in FDT
    printf("Opening FIFOs...\n");

    int FIFOSERVER = open(path2ServerFIFO, O_RDWR, S_IWUSR);

    // Checks on errors opning FIFOCLIENT/FIFOSERVER
    if(FIFOSERVER == -1) {
        errExit("<Client Request> open FIFOSERVER failed");
    }

    // Creates the Shared Memory key
    key_t shmKey = ftok("src/server.c", 's');

    // Checks if ftok succesfully created a key
    if(shmKey == -1) {
        errExit("<Server> shared memory ftok failed");
    }

    // Creates the shared memory
    int shmid = shmget(shmKey, sizeof(Node_t *) * 100, IPC_CREAT | S_IRUSR | S_IWUSR);
  
    // Checks if the shared memory was successfully created
    if(shmid == -1) {
        errExit("<Server> shmget failed");
    }

    // Create the semaphore set
    // int semid = semget(shmKey, 1, IPC_CREAT | S_IRUSR | S_IWUSR);

    // Signal set
    sigset_t noSIGTERMSet;

    // Fill the signal set
    sigfillset(&noSIGTERMSet);

    // Deletes SIGTERM from mySet
    sigdelset(&noSIGTERMSet, SIGTERM);

    // Blocks all signals but SIGTERM
    sigprocmask(SIG_SETMASK, &noSIGTERMSet, NULL);

    // Creates the child process KeyManager
    pid_t key_manager = fork();

    // ========== SERVER OPERATION SECTION ==========
    // sigHandler as handler for SIGTERM
    if(signal(SIGTERM, sigHandler) == SIG_ERR) {
        errExit("<Server> signal failed");
    }

    // Request read from FIFO
    Request_t *request = NULL;
    
    // Reads the request from FIFOSERVER
    if(read(FIFOSERVER, request, sizeof(Request_t *)) == -1) {
        errExit("<Server> read from FIFOSERVER failed");
    }

    // Response containing the user key
    Response_t *user_key = NULL;

    // Generate user_key
    if(strcmp(request -> service, "Stampa") >= 0 || strcmp(request -> service, "Salva") >= 0 || strcmp(request -> service, "Invia") >= 0) {
        generate_key(request, user_key);
    }

    // ================= KEYMANAGER ===============
    if(key_manager == 0) {
        // Attach the shared memory segment
        List_t *attached_shm_list = (List_t *) shmat(shmid, NULL, 0);

        if(attached_shm_list == (List_t *) -1) {
            errExit("<KeyMamager> shmat failed");
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
                errExit("<KeyManager> shmdt failed");
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
            errExit("<Server> shmat failed");
        }

        // Inserts the new node in the shared memory
        insert_list(attached_shm_list, request -> id, user_key);

        // Opens FIFOCLIENT and add it to the FDT
        FIFOCLIENT = open(path2ClientFIFO, O_RDWR, S_IRUSR);

        // Checks if open happened
        if(FIFOCLIENT == -1) {
            errExit("<Server> open FIFOCLIENT failed");
        }

        // Writes the response on the FIFO
        if(write(FIFOCLIENT, user_key, sizeof(Response_t *)) == 0) {
            errExit("<Server> write on FIFOSERVER failed");
        }
    } else {
        errExit("<Server> NULL user key");
    }

    // Waits for SIGTERM
    // TODO Catch di SIGTERM per uscire dal programma
    // =======================================
    // Detach this process from the Shared Memory
    if(shmdt(attached_shm_list) == -1) {
        errExit("<Server> shmdt failed");
    }

    // Deletes the shared memory
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        errExit("<Server> shmctl IPC_RMID failed");
    }

    // Remove the file that contains the shmid
    if(remove("../../shm_id.txt") == -1) {
        errExit("<Server> remove failed");
    }

    // Checks on errors closing FIFOSERVER file descriptor
    if(close(FIFOSERVER) == -1) {
        errExit("<Server> close FIFOSERVER failed");
    }

    // Checks on errors closing FIFOCLIENT file descriptor
    if(close(FIFOCLIENT) == -1) {
        errExit("<Server> close FIFOCLIENT failed");
    }

    // Checks fo errors unlinking the FIFO
    if(unlink(path2ServerFIFO) == -1) {
        errExit("<Server> unlink FIFOSERVER failed");
    }
    // =======================================

    return 0;
}