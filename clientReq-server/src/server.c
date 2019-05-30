#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>

#include "../../utils/include/errExit.h"
#include "../include/server.h"
#include "../include/clientReq.h"
#include "../../utils/include/list_utils.h"

int main (int argc, char *argv[]) {
    // Path to FIFOSERVER/FIFOCLIENT location in filesystem
    char *path2ServerFIFO = "FIFOs/FIFOSERVER";
    char *path2ClientFIFO = "FIFOs/FIFOCLIENT";

    // FIFOSERVER file desctiptor
    int FIFOSERVER;

    // FIFOCLIENT file descriptor
    int FIFOCLIENT;

    // Checks on error making FIFOSERVER
    if(mkfifo(path2ServerFIFO, S_IWUSR | S_IRUSR) == -1) {
        errExit("<Server> mkfifo FIFOSERVER falied");
    }

    // Opens FIFOSERVER/FIFOCLIENT and place them in FDT
    FIFOSERVER = open(path2ServerFIFO, O_RDONLY);
    FIFOCLIENT = open(path2ClientFIFO, O_WRONLY);

    // Creates the Shared Memory key
    key_t shmKey = ftok(IPC_PRIVATE, 's');

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
    Request_t *request;
    
    // Reads the request from FIFOSERVER
    if(read(FIFOSERVER, request, sizeof(Request_t *)) == -1) {
        errExit("<Server> read from FIFOSERVER failed");
    }

    // Response containing the user key
    Response_t *user_key;

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

            if(signal(SIGTERM, sigHandler) == SIG_ERR) {
                errExit("<KeyManager> signal failed");
            }
        }
    }
    // =============================================

    if(user_key != NULL) {
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

    // Deletes the shared memory
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        errExit("<Server> shmctl IPC_RMID failed");
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

    return 0;
}

// Generates a user_key
void generate_key(Request_t *request, Response_t *response) {
    // Init random seed
    srand(time(NULL));

    // Generates 4 random integers
    for(int i = 0; i < 4; i++) {
        response -> user_key_numeric[i] = rand() % 10;
    }

    // Checks for service type and assigns service part of user_key
    if(strcmp(request -> service, "Stampa") == 0) {
        strcpy(response -> user_key_service, "prt");
    } else if(strcmp(request -> service, "Salva") == 0) {
        strcpy(response -> user_key_service, "sav");
    } else {
        strcpy(response -> user_key_service, "snd");
    }
}

// Function to print user_key
void print_key(Response_t *response) {
    for(int i = 0; i < 4; i++) {
        printf("%i", response -> user_key_numeric[i]);
    }

    printf("%s\n", response -> user_key_service);
}

// Handles signal 
void sigHandler(int sig) {
    printf("SIGTERM occourred");
}

// Checks if five minutes difference sussist between 2 timestamps
int check_five_min_diff(struct timeval *current, struct timeval *data_timestamp) {
    if((current -> tv_sec) - (data_timestamp -> tv_sec) >= 300) {
        return 1;
    }

    return 0;
}

// Compare two different data
int check_eq_data(Data_t *data1, Data_t *data2) {
    if(strcmp(data1 -> id, data2 -> id) == 0 && 
        strcmp(userkey_to_string(data1 -> user_key), userkey_to_string(data2 -> user_key)) == 0 && 
        data1 -> timestamp.tv_sec == data2 -> timestamp.tv_sec
      ){

        return 1;
    }

    return 0;
}

// Coverts *user_key to a string
char *userkey_to_string(Response_t *user_key) { 
    char numeric[5];
    int tmp = atoi(user_key -> user_key_numeric);

    sprintf(numeric, "%d", tmp);

    return strcat(numeric, user_key -> user_key_service);
}