#ifndef GLOBALS_H
#define GLOBALS_H

#include <unistd.h>

#include "./list_lib.h"

extern pid_t son_process_pid;
extern List_t *attached_shm_list;
extern int shm_id;
extern int sem_id;
extern const char *path_to_client_FIFO;
extern const char *path_to_server_FIFO;

#endif