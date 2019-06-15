#include <fcntl.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#include "../../lib/include/err_lib.h"
#include "../../lib/include/msg_lib.h"

int main (int argc, char *argv[]) {
    // Stores the message que key (first argument) in a local variable
    int msg_key = atoi(argv[1]);

    // Checks if the key is valid
    if(msg_key <= 0) {
        err_exit("<Send> msgKey must be greater than 0");
    }

    // Gets the message queue
    int msqid = msgget(msg_key, S_IRUSR | S_IWUSR);

    int i = 2;
    char *string2send = "";

    // Loop to concat strings
    while(argv[i] != NULL) {
        // Concat the strings to send them as an unique message
        strcat(string2send, argv[i]);

        // Adds a space between arguments if they're not the last one
        if(argv[i + 1] != NULL) {
            strcat(string2send, " ");
        }

        i++;
    }

    // Creates the message
    Message_t *message;

    // Set the Message_t values
    message -> mtype = 1;
    strcpy(message -> mtext, string2send);

    // Sends teh message
    if(msgsnd(msqid, message, sizeof(message), 0) == -1) {
        err_exit("<Send> msgsnd failed");
    }

    return 0;
}
