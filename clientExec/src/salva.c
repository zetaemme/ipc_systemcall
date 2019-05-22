#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../../clientReq-server/inc/errExit.h"

int main (int argc, char *argv[]) { 
    // Generate the new file's filename
    char *filename = strcat(argv[0], ".txt");

    // Creates exclusively the new file
    int file = open(filename, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);

    if(file == -1) {
        errExit("open failed");
    }

    // Buffer that contains the string and while loop index
    char buffer[256];
    int i = 3;

    while(argv[i] != NULL) {
        strcpy(buffer, argv[i]);

        if(write(file, buffer, sizeof(argv[i]))) {
            errExit("write failed");
        }

        i++;
    }

    printf("Save complete!\n");

    return 0;
}
