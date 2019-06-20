#include <unistd.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>

#include "../../lib/include/list_lib.h"
#include "../../lib/include/str_lib.h"
#include "../../lib/include/err_lib.h"
#include "../../lib/include/sem_lib.h"
#include "../../lib/include/exec_lib.h"

int main (int argc, char *argv[]) {
    // argv[1] = id, argv[2] = user_key, argv[3] = args

    if(argc < 4) {
        err_exit("<Exec> Not enough arguments");
    }

    int integer_user_key = atoi(argv[2]);

    // Creating a subset array containing the arguments for the execl syscall
    char **args = (char **) malloc((argc - 3) * sizeof(char));

    int j = 0;
    for(int i = 3; i < argc; i++) {
        strcpy(args[j], argv[i]);
        j++;
    }

    // Gets the shared memory key
    printf("Generating shm key...\t\t");

    key_t shm_key = ftok("../../clientReq-server/src/server.c", 1);

    if(shm_key == -1) {
        err_exit("\n<Exec> ftok failed");
    } else {
        printf("DONE!\n");
    }

    // Gets the shared memory id
    printf("Getting Shared Memory...\t\t");

    int shm_id = shmget(shm_key, sizeof(Node_t *) * 100, S_IRUSR | S_IWUSR);

    if(shm_id == -1) {
        err_exit("\n<Exec> shmget failed");
    } else {
        printf("DONE!\n");
    }

    // Attach the shared memory to the linked list
    List_t *attached_shm_list = (List_t *) shmat(shm_id, NULL, 0);

    if(attached_shm_list != (List_t *)1) {
        err_exit("<Exec> shmat failed");
    }

    // Gets the sem key
    printf("Generating sem key...\t\t");

    int sem_key = ftok("../../clientReq-server/src/server.c", 2);

    if(sem_key == -1) {
        err_exit("\n<Exec> ftok failed");
    } else {
        printf("DONE!\n");
    }

    // Gets the semaphore
    printf("Getting semaphore...\t\t");

    int sem_id = semget(sem_key, 1, S_IRUSR | S_IWUSR);

    if(sem_id == -1) {
        err_exit("\n<Exec> semget failed");
    } else {
        printf("DONE!\n");
    }

    // =========== OPERAZIONE PROTETTA ===========

    // Semaphore protects the operations below
    semOp(sem_id, 0, -1);

    // Current node on the shared memory list
    Node_t *current = attached_shm_list -> head;

    int validity_flag = 0;

    // Every int will fit into a 12 chars array without overflow
    char string_userkey[12]; 

    // Checks for user_key validity
    while(current -> next != NULL) {
        if(current -> user_key == integer_user_key && current ->  has_been_used != 0) {
            validity_flag = 1;
            current -> has_been_used = 1;
        }
    }

    // Shared Memory is now accessible to everyone who wants
    semOp(sem_id, 0, 1);

    // =========================================

    int service = get_first_digit(integer_user_key); 

    if(validity_flag == 1) {
        if(service == 1) {
            if(execl("stampa", "stampa", *args, (char *) NULL) == -1) {
                err_exit("<Exec> failed to execute 'Print'");
            }
        } else if(service == 2) {
            if(execl("invia", "invia", *args, (char *) NULL) == -1) {
                err_exit("<Exec> failed to execute 'Send'");
            }
        } else {
            if(execl("salva", "salva", *args, (char *) NULL) == -1) {
                err_exit("<Exec> failed to execute 'Save'");
            }
        }
    } else {
        err_exit("<Exec> Error, invalid user key!");
    }

    // Detachs ClientExec from the Shared Memory
    if(shmdt(attached_shm_list) == -1) {
        err_exit("<ClientExec> shmdt failed");
    }

    return 0;
}