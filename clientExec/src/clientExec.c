#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#include "../../utils/include/list_utils.h"
#include "../include/clientExec.h"
#include "../../clientReq-server/include/server.h"
#include "../../utils/include/errExit.h"

int main (int argc, char *argv[]) {
    // argv[1] = id, argv[2] = user_key, argv[3] = args

    // Slices the id to get the service reference part
    char service[4];
    str_slice(argv[1], service, 4, 8);

    // Creating a subset array containing the arguments for the execl syscall
    char **args = (char **) malloc((argc - 3) * sizeof(char));

    int j = 0;
    for(int i = 3; i < argc; i++) {
        strcpy(args[j], argv[i]);

        j++;
    }

    // Gets the shared memory id
    key_t shmKey = ftok("../../clientReq-server/src/server.c", 's'); 
    int shmid = shmget(shmKey, sizeof(Node_t *) * 100, S_IRUSR | S_IWUSR);

    // Attach the shared memory to the linked list
    List_t *attached_shm_list = (List_t *) shmat(shmid, NULL, 0);

    Node_t *current = attached_shm_list -> head;

    int validity_flag = 0;

    // Checks for user_key validity
    while(current -> next != NULL) {
        if(strcmp(userkey_to_string(current -> value -> user_key), argv[2]) == 0) {
            validity_flag = 1;
        }
    } 

    // Simula validazione user_key(argv[2])
    if(validity_flag == 1) {
        if(strcmp(service, "prt") == 0) {
            if(execl("./stampa.c", *args, (char *) NULL) == -1) {
                errExit("<Exec> failed to execute 'Print'");
            }
        } else if(strcmp(service, "snd") == 0) {
            if(execl("./invia.c", *args, (char *) NULL) == -1) {
                errExit("<Exec> failed to execute 'Send'");
            }
        } else {
            if(execl("./salva.c", *args, (char *) NULL) == -1) {
                errExit("<Exec> failed to execute 'Save'");
            }
        }
    }

    // Detachs ClientExec from the Shared Memory
    if(shmdt(attached_shm_list) == -1) {
        errExit("<ClientExec> shmdt failed");
    }

    return 0;
}

// Slices src string from start to end and saves it into dest
void str_slice(const char *src, char *dest, size_t start, size_t end) {
    size_t j = 0;

    for (size_t i = start; i <= end; ++i) {
        dest[j++] = src[i];
    }
}
