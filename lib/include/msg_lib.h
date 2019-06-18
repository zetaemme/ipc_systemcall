#ifndef MSG_LIB_H
#define MSG_LIB_H

#include <sys/stat.h>
#include <sys/msg.h>

#include "./err_lib.h"

typedef struct Message_s {
    long mtype;
    char mtext[256];
} Message_t;

void send_message(int msqid, Message_t *msg, unsigned int msg_size, int flag);
int get_message_queue(int message_key);

#endif