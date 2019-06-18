#include "../include/msg_lib.h"

void send_message(int msqid, Message_t *msg, unsigned int msg_size, int flag) {
    if(msgsnd(msqid, msg, msg_size, 0) == -1) {
        err_exit("<Send> msgsnd failed");
    }
}