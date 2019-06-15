#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>

#include "../../lib/include/err_lib.h"
#include "../../lib/include/server_lib.h"
#include "../../lib/include/list_lib.h"
#include "../../lib/include/request_lib.h"
#include "../../lib/include/str_lib.h"
#include "../../lib/include/sig_lib.h"
#include "../../lib/include/sem_lib.h"

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

    int FIFOSERVER = open(path_to_server_FIFO, O_CREAT | O_RDWR, S_IWUSR);

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

    // Create the semaphore set
    int sem_id = semget(shm_key, 1, IPC_CREAT | S_IRUSR | S_IWUSR);

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
    if(signal(SIGTERM, server_sig_handler) == SIG_ERR) {
        err_exit("<Server> signal failed");
    }

    // Executes untill SIGTERM
    while(1) {
        // Request read from FIFO
        Request_t *request = NULL;

        printf("Reading from FIFOSERVER...\t\t");

        // Reads the request from FIFOSERVER
        if(read(FIFOSERVER, request, sizeof(Request_t *)) == -1) {
            err_exit("<Server> read from FIFOSERVER failed");
        } else {
            printf("DONE!\n");
        }

        // Response containing the user key
        Response_t *user_key = NULL;

        // Turn the request -> service lowercase
        char *lower_service = request -> service;
        lower_case(lower_service);

        // Generate user_key
        if(strcmp(lower_service, "stampa") >= 0 || strcmp(lower_service, "salva") >= 0 || strcmp(lower_service, "invia") >= 0) {
            generate_key(request, user_key);
        }

        printf("Forking KeyManager...\t\t");

        // Creates the child process KeyManager
        pid_t key_manager = fork();

        // ================= KEYMANAGER ===============
        if(key_manager == 0) {

            printf("Key manager is running...\n\n");

            // Attach the shared memory segment
            List_t *attached_shm_list = (List_t *) shmat(shm_id, NULL, 0);

            if(attached_shm_list == (List_t *) -1) {
                err_exit("<Key Manager> shmat failed");
            }

            // Now-time value
            struct timeval current_time;

            // Current node of the list
            Node_t *current_node = attached_shm_list -> head;

            if(signal(SIGTERM, km_sig_handler) == SIG_ERR) {
                err_exit("<Key Manager> signal failed");
            }

            // This loop executes each 30 seconds
            while(1) {
                // Gets the current time
                gettimeofday(&current_time, NULL);

                while(current_node -> next != NULL) {
                    if(check_five_min_diff(&current_time, &(current_node -> value) -> timestamp)) {
                        delete_from_list(attached_shm_list, current_node);

                        current_node = current_node -> next;
                    } else {
                        current_node = current_node -> next;
                    }
                }

                // Sleep for 30 seconds
                sleep(30);

                // Reset the current node to the head of the list
                current_node = attached_shm_list -> head;
            }
        }
        // =============================================

        // Attach the server to the Shared Memory
        List_t *attached_shm_list = (List_t *) shmat(shm_id, NULL, 0);

        // FIFOCLIENT file descriptor
        int FIFOCLIENT;

        if(user_key != NULL) {
            if(attached_shm_list == (List_t *) -1) {
                err_exit("<Server> shmat failed");
            }

            // Inserts the new node in the shared memory
            insert_list(attached_shm_list, request -> id, user_key);

            // Opens FIFOCLIENT and add it to the FDT
            FIFOCLIENT = open(path_to_client_FIFO, O_RDWR, S_IRUSR);

            // Checks if open happened
            if(FIFOCLIENT == -1) {
                err_exit("<Server> open FIFOCLIENT failed");
            }

            // Writes the response on the FIFO
            if(write(FIFOCLIENT, user_key, sizeof(Response_t *)) == 0) {
                err_exit("<Server> write on FIFOSERVER failed");
            }
        } else {
            err_exit("<Server> NULL user key");
        }
    }

    // Waits for SIGTERM

    return 0;
}
