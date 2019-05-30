#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../include/clientExec.h"
#include "../../utils/include/errExit.h"

int main (int argc, char *argv[]) {
    // TODO Accesso a segmento di Shared Memory e validazione dati

    // argv[1] = id, argv[2] = user_key, argv[3] = args

    // Slices the id to get the service reference part
    char service[4];
    str_slice(argv[1], service, 4, 8);

    // Creating a subset array containing the arguments for the execl syscall
    char **args = (char **) malloc((argc - 3) * sizeof(char));

    int j = 0;
    for(int i = 3; i < argc; i++) {
        strcpy(args[j], argv[i]);

        j++;
    }

    // Simula validazione user_key(argv[2])
    if(strcmp(argv[2], "1234") == 0) {
        if(strcmp(service, "prt") == 0) {
            if(execl("./stampa.c", *args, (char *) NULL) == -1) {
                errExit("<Exec> failed to execute 'Print'");
            }
        } else if(strcmp(service, "snd") == 0) {
            if(execl("./invia.c", *args, (char *) NULL) == -1) {
                errExit("<Exec> failed to execute 'Send'");
            }
        } else {
            if(execl("./salva.c", *args, (char *) NULL) == -1) {
                errExit("<Exec> failed to execute 'Save'");
            }
        }
    }

    return 0;
}

void str_slice(const char *src, char *dest, size_t start, size_t end) {
    size_t j = 0;

    for (size_t i = start; i <= end; ++i) {
        dest[j++] = src[i];
    }
}
