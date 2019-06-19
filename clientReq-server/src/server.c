#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../../lib/include/err_lib.h"
#include "../../lib/include/server_lib.h"
#include "../../lib/include/list_lib.h"
#include "../../lib/include/sem_lib.h"
#include "../../lib/include/sig_lib.h"
#include "../../lib/include/globals.h"

// Global for the son's PID
pid_t son_process_pid;

// Flag to control the while loop (volatile because it can change asyncronously)
volatile int sig_caught = 0;

// TODO Cercare un modo per gestire le global e sig_handler

int main (int argc, char *argv[]) {
    // Path to the fifo files
    static const char *path_to_client_FIFO = "./FIFO/FIFOCLIENT";
    static const char *path_to_server_FIFO = "./FIFO/FIFOSERVER";

    // Checks on error making FIFOSERVER
    printf("Creating FIFOSERVER...\t\t");

    if(mkfifo(path_to_server_FIFO, S_IWUSR | S_IRUSR) == -1) {
        err_exit("<Server> mkfifo FIFOSERVER failed");
    } else {
        printf("DONE!\n");
    }

    // Opens FIFOSERVER/FIFOCLIENT and place them in FDT
    printf("Opening FIFOSERVER...\t\t");

    // FIFOCLIENT and FIFOSERVER file descriptors
    int FIFOSERVER = open(path_to_server_FIFO, O_CREAT | O_RDWR, S_IWUSR);
    int FIFOCLIENT;

    // Checks on errors opning FIFOCLIENT/FIFOSERVER
    if(FIFOSERVER == -1) {
        err_exit("<Client Request> open FIFOSERVER failed");
    } else {
        printf("DONE!\n");
    }

    printf("Generating shm key...\t\t");

    // Creates the Shared Memory key
    key_t shm_key = ftok("src/server.c", 's');

    // Checks if ftok succesfully created a key
    if(shm_key == -1) {
        err_exit("<Server> shared memory ftok failed");
    } else {
        printf("DONE!\n");
    }

    printf("Getting Shared Memory...\t");

    // Creates the shared memory
    int shm_id = shmget(shm_key, sizeof(Node_t *) * 100, IPC_CREAT | S_IRUSR | S_IWUSR);

    // Checks if the shared memory was successfully created
    if(shm_id == -1) {
        err_exit("\n\n<Server> shmget failed");
    } else {
        printf("DONE!\n");
    }

    // TODO Togliere commento per utilizzare semafori
    // Create the semaphore set
    // int sem_id = semget(shm_key, 1, IPC_CREAT | S_IRUSR | S_IWUSR);

    printf("Generating SigSet...\t\t");

    // Signal set
    sigset_t no_SIGTERM_set;

    // Fill the signal set
    sigfillset(&no_SIGTERM_set);

    // Deletes SIGTERM from mySet
    sigdelset(&no_SIGTERM_set, SIGTERM);

    // Blocks all signals but SIGTERM
    sigprocmask(SIG_SETMASK, &no_SIGTERM_set, NULL);

    // ========== SERVER OPERATION SECTION ==========
    // sigHandler as handler for SIGTERM
    if(signal(SIGTERM, sig_handler) == SIG_ERR) {
        err_exit("<Server> signal failed");
    }

    // ============================ MAIN EXECUTION ============================

    List_t *attached_shm_list;

    // Executes untill SIGTERM
    while(!sig_caught) {

        // TODO Gestire in modo che se non legge niente da FIFOCLIENT non da SEGFAULT (Dio bestia)

        // Request read from FIFO
        Request_t request;

        printf("Reading from FIFOSERVER...\t\t");

        // Reads the request from FIFOSERVER
        if(read(FIFOSERVER, &request, sizeof(Request_t *)) == -1) {
            err_exit("<Server> read from FIFOSERVER failed");
        } else {
            printf("DONE!\n");
        }

        // Response containing the user key
        Response_t user_key;

        // Generate user_key
        if(strcmp(request.service, "stampa") >= 0 || strcmp(request.service, "salva") >= 0 || strcmp(request.service, "invia") >= 0) {
            generate_key(&request, &user_key);
        }

        // Attach the server to the Shared Memory
        attached_shm_list = (List_t *) shmat(shm_id, NULL, 0);

        if(attached_shm_list == (List_t *) -1) {
            err_exit("<Server> shmat failed");
        }

        if(!(strcmp(user_key.user_key_service, "") >= 0)) {
            // Inserts the new node in the shared memory
            insert_list(attached_shm_list, request.id, &user_key);

            // Opens FIFOCLIENT and add it to the FDT
            FIFOCLIENT = open(path_to_client_FIFO, O_RDWR, S_IRUSR);

            // Checks if open happened
            if(FIFOCLIENT == -1) {
                err_exit("<Server> open FIFOCLIENT failed");
            }

            // Writes the response on the FIFO
            if(write(FIFOCLIENT, &user_key, sizeof(Response_t *)) == 0) {
                err_exit("<Server> write on FIFOSERVER failed");
            }
        } else {
            err_exit("<Server> NULL user key");
        }

        // Creates the child process KeyManager
        son_process_pid = fork();

        // ================= KEYMANAGER ===============
        if(son_process_pid == 0) {

            printf("Key manager is running...\n\n");

            // Attach the shared memory segment
            List_t *km_attached_shm_list = (List_t *) shmat(shm_id, NULL, 0);

            if(km_attached_shm_list == (List_t *) -1) {
                err_exit("<Key Manager> shmat failed");
            }

            // Now-time value
            struct timeval current_time;

            // Current node of the list
            Node_t *current_node = attached_shm_list -> head;

            // This loop executes each 30 seconds
            while(1) {
                // Gets the current time
                gettimeofday(&current_time, NULL);

                while(current_node -> next != NULL) {
                    if(check_five_min_diff(&current_time, &(current_node -> value) -> timestamp)) {
                        delete_from_list(km_attached_shm_list, current_node);

                        current_node = current_node -> next;
                    } else {
                        current_node = current_node -> next;
                    }
                }

                // Sleep for 30 seconds
                sleep(30);

                // Reset the current node to the head of the list
                current_node = km_attached_shm_list -> head;
            }
        }
        // =============================================

        // Waits for SIGTERM
        int status;
        wait(&status);

    }

    // Detach the list associated with the shared memory
    if(shmdt(attached_shm_list) == -1) {
        err_exit("<Server> shmdt failed");
    }

    // Deletes the shared memory
    if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
        err_exit("<Server> shmctl IPC_RMID failed");
    }

    // Checks on errors closing FIFOSERVER file descriptor
    if(close(FIFOSERVER) == -1) {
        err_exit("<Server> close FIFOSERVER failed");
    }

    // Checks on errors closing FIFOCLIENT file descriptor
    if(close(FIFOCLIENT) == -1) {
        err_exit("<Server> close FIFOCLIENT failed");
    }

    // Checks fo errors unlinking the FIFO
    if(unlink(path_to_server_FIFO) == -1) {
        err_exit("<Server> unlink FIFOSERVER failed");
    }

    return 0;
}
