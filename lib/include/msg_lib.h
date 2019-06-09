#ifndef MSG_LIB_H
#define MSG_LIB_H

typedef struct Message_s {
    long mtype;
    char mtext[256];
} Message_t;

#endif