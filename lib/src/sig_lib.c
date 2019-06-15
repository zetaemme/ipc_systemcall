#include "../include/sig_lib.h"

// Handles SIGTERM for server.c
void server_sig_handler(int sig, List_t* shared_memory_list, int FIFOSERVER, int FIFOCLIENT, int shm_id, const char *path_to_server_FIFO) {
    if(shmdt(shared_memory_list) == -1) {
        errExit("<Server> shmdt failed");
    }

    // Deletes the shared memory
    if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
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
    if(unlink(path_to_server_FIFO) == -1) {
        errExit("<Server> unlink FIFOSERVER failed");
    }
}

// Handles SIGTERM for server.c/KeyManager
void km_sig_handler(int sig, List_t* shared_memory_list) {
    if(shmdt(shared_memory_list) == -1) {
        errExit("<Key Manager> shmdt failed");
    }
}
