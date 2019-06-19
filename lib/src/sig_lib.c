#include "../include/sig_lib.h"

void server_sig_handler(int sig) {
    if(sig == SIGTERM) {
        // Killing KeyManager
        printf("Closing KeyManager...\t\t");

        if(kill(son_process_pid, SIGTERM)) {
            err_exit("\n<Server Signal Handler> kill failed");
        }  else {
            printf("DONE!\n");
        }

        // Removes FIFOSERVER
        printf("Removing FIFOSERVER...\t\t");

        path_to_server_FIFO = "./FIFO/FIFOSERVER";

        if(unlink(path_to_server_FIFO) == -1) {
            err_exit("\n<Server SIgnal Handler> unlink failed");
        } else {
            printf("DONE!\n");
        }

        // Removes Shared Memory
        printf("Removing Shared Memory...\t\t");

        if(shmdt(attached_shm_list) == -1 || shmctl(shm_id, IPC_RMID, NULL) == -1) {
            err_exit("\n<Server Signal Handler> failed to remove Shared Memory");
        } else {
            printf("DONE!\n");
        }

        // Removes Semaphore
        printf("Removing Semaphore...\t\t");

        if(semctl(sem_id, 0, IPC_RMID, NULL) == -1) {
            err_exit("\n<Server Signal Handler> semctl IPC_RMID failed");
        } else {
            printf("DONE!\n");
        }

        //Wait for KeyManager to close
        if(waitpid(son_process_pid, NULL, 0) == -1) {
            err_exit("<Server Signal Handler> waitpid failed");
        }

        exit(0);
    }
}

void km_sig_handler(int sig) {
    if(sig == SIGTERM) {
        printf("<Key Manager> Shutting down...\n");
        exit(0);
    }
}