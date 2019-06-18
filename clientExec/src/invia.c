#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "../../lib/include/err_lib.h"
#include "../../lib/include/msg_lib.h"

int main (int argc, char *argv[]) {
    // Stores the message que key (first argument) in a local variable
    int msg_key = atoi(argv[1]);

    // Checks if the key is valid
    if(msg_key <= 0) {
        err_exit("<Send> msg_key must be greater than 0");
    }

    // Gets the message queue
    int msqid = get_message_queue(msg_key);

    int i = 2;
    char *string_to_send = "";

    // Loop to concat strings
    while(argv[i] != NULL) {
        // Concat the strings to send them as an unique message
        strcat(string_to_send, argv[i]);

        // Adds a space between arguments if they're not the last one
        if(argv[i + 1] != NULL) {
            strcat(string_to_send, " ");
        }

        i++;
    }

    // Creates the message
    Message_t message;

    // Set the Message_t values
    message.mtype = 1;
    strcpy(message.mtext, string_to_send);

    // Sends the message
    send_message(msqid, &message, sizeof(message), 0);

    return 0;
}
