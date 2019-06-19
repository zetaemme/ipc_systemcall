#include "../include/msg_lib.h"

void send_message(int msqid, Message_t *msg) {
    if(msgsnd(msqid, msg, sizeof(msg -> mtext), 0) == -1) {
        err_exit("<Send> msgsnd failed");
    }
}

int get_message_queue(int message_key) {
    int msqid = msgget(message_key, S_IWUSR);

    if(msqid == -1) {
        err_exit("<Send> msgget failed");
    }

    return msqid;
}