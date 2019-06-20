#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../../lib/include/err_lib.h"
#include "../../lib/include/server_lib.h"
#include "../../lib/include/list_lib.h"
#include "../../lib/include/sem_lib.h"
#include "../../lib/include/globals.h"
#include "../../lib/include/sig_lib.h"

// Global for the son's PID
pid_t son_process_pid;

// Global for the Shared Memory list
Node_t *attached_shm_list;

// Global for Shared Memory id
int shm_id;

// Global for Semaphore id
int sem_id;

// Globals for FIFOs paths
const char *path_to_server_FIFO;
const char *path_to_client_FIFO;

int main (int argc, char *argv[]) {
    path_to_server_FIFO = "./FIFO/FIFOSERVER";
    path_to_client_FIFO = "./FIFO/FIFOCLIENT";

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

    // Checks on error making FIFOSERVER
    printf("Creating FIFOSERVER...\t\t");

    if(mkfifo(path_to_server_FIFO, S_IWUSR | S_IRUSR) == -1) {
        err_exit("<Server> mkfifo FIFOSERVER failed");
    } else {
        printf("DONE!\n");
    }

    printf("Generating shm key...\t\t");

    // Creates the Shared Memory key
    key_t shm_key = ftok("src/server.c", 300);

    // Checks if ftok succesfully created a key
    if(shm_key == -1) {
        err_exit("<Server> shared memory ftok failed");
    } else {
        printf("DONE!\n");
    }

    printf("Getting Shared Memory...\t");

    // Creates the shared memory
    shm_id = shmget(shm_key, sizeof(Node_t) * 20, IPC_CREAT | S_IRUSR | S_IWUSR);

    // Checks if the shared memory was successfully created
    if(shm_id == -1) {
        err_exit("\n\n<Server> shmget failed");
    } else {
        printf("DONE!\n");
    }

    // Shared Memory list
    attached_shm_list = (Node_t *) shmat(shm_id, NULL, 0);

    if(attached_shm_list == (Node_t *) -1) {
        err_exit("<Server> shmat failed");
    }

    // Create the semaphore set key
    printf("Generating sem key...\t\t");

    key_t sem_key = ftok("src/clientReq.c", 100);

    // Checks if the key has been generated
    if(sem_key == -1) {
        err_exit("<Server> semaphore ftok failed");
    } else {
        printf("DONE!\n");
    }

    // Crate a semaphore set with 1 semaphore
    sem_id = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    // Assigns the value for the semaphore
    union semun arg;
    arg.val = 1;

    if(semctl(sem_id, 0, SETVAL, arg) == -1) {
        err_exit("<Server> semctl SETVAL failed");
    } 

    // ============================ MAIN EXECUTION ============================

    son_process_pid = fork();

    
    if(son_process_pid == 0) {
        // ================= KEYMANAGER ===============
        printf("\nKey manager is running...\n\n");

        if(signal(SIGTERM, km_sig_handler) == SIG_ERR) {
            err_exit("<Key Manager> signal failed");
        }

        // Attach the shared memory segment
        Node_t *km_attached_shm_list = (Node_t *) shmat(shm_id, NULL, 0);

        if(km_attached_shm_list == (Node_t *) -1) {
            err_exit("<Key Manager> shmat failed");
        }

        // Current timestamp
        time_t current_time;

        // This loop executes each 30 seconds
        while(1) {
            sleep(30);

            // =========== OPERAZIONE PROTETTA ===========

            // Semaphore protects the operations below
            semOp(sem_id, 0, -1);

            // Current node of the list
            Node_t *current_node = attached_shm_list;

            while(current_node != attached_shm_list + 20) {
                // Gets the current time
                current_time = get_timestamp() - 300;

                if(current_node -> timeval < current_time && current_node -> timeval != 0) {
                    delete_from_shared_memory(current_node);
                }

                ++current_node;
            }

            // Shared Memory is now accessible to everyone who wants
            semOp(sem_id, 0, 1);

            // =========================================
        }
        // ============================================       
    } else if(son_process_pid == -1) {
        err_exit("<Server> fork failed");
    } else {
        // ================= SERVER ===================
        while(1) {
            // FIFOSERVER file descriptor
            int FIFOSERVER = open(path_to_server_FIFO, O_RDWR);
          
            // Checks on errors opning FIFOCLIENT/FIFOSERVER
            if(FIFOSERVER == -1) {
                err_exit("\n<Client Request> open FIFOSERVER failed");
            }

            // Request read from FIFO
            Request_t bR[1];

            // Reads the request from FIFOSERVER
            if(read(FIFOSERVER, bR, sizeof(bR)) == -1) {
                err_exit("\n<Server> read from FIFOSERVER failed");
            }

            // Response containing the user key
            Response_t user_key;

            Request_t request;

            strcpy(request.id, bR -> id);
            strcpy(request.service, bR -> service);

            // Generate user_key
            if(generate_key(request.service, &user_key) != 1) {
                err_exit("\n<Server> generate key error");
            } 

            // =========== OPERAZIONE PROTETTA ===========

            // Semaphore protects the operations below
            semOp(sem_id, 0, -1);

            // Inserts the new node in the shared memory
            write_in_shared_memory(attached_shm_list, request.id, user_key.user_key);

            // Shared Memory is now accessible to everyone who wants
            semOp(sem_id, 0, 1);

            // =========================================
        
            // FIFOCLIENT file descriptor
            int FIFOCLIENT = open(path_to_client_FIFO, O_RDWR);

            // Checks if open happened
            if(FIFOCLIENT == -1) {
                err_exit("\n<Server> open FIFOCLIENT failed");
            }

            // Write buffer
            Response_t bW[1];

            bW -> user_key = user_key.user_key; 

            if(write(FIFOCLIENT, bW, sizeof(bW)) != sizeof(bW)) {
                err_exit("\n<Server> write on FIFOSERVER failed");
            }

            if(close(FIFOCLIENT) == -1) {
                err_exit("\n<Server> close failed");
            }

            printf("\n");
        }
        // ============================================
    }
    // ========================================================================

    return 0;
}
