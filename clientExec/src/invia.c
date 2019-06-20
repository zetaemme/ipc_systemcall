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

    if(argc < 3) {
        err_exit("<Send> Not enough argumets");
    }

    // Gets the message queue
    int msq_id = get_message_queue(msg_key);

    int i = 2;
    char string_to_send[] = argv[2];

    // Creates the message
    Message_t message;

    // Set the Message_t values
    message.mtype = 1;
    strcpy(message.mtext, string_to_send);

    // Sends the message
    send_message(msq_id, &message);

    printf("Message sent!\n\n");

    return 0;
}
