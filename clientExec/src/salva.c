#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../../lib/include/err_lib.h"

int main(int argc, char *argv[]) { 
    // Generate the new file's filename
    char *filename = strcat(argv[0], ".txt");

    printf("Creating file...\t\t");

    // Creates exclusively the new file
    int file = open(filename, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);

    if(file == -1) {
        errExit("\n<Exec> open failed");
    } else {
        printf("DONE!\n");
    }

    // ============= QUI C'E' QUALCHE PROBLEMA =====================

    // TODO

    // Buffer that contains the string and while loop index
    char buffer[256];
    int i = 3;

    while(argv[i] != NULL) {
        strcpy(buffer, argv[i]);

        if(write(file, buffer, sizeof(argv[i]))) {
            errExit("<Exec> write failed");
        }

        i++;
    }

    // ==============================================================

    printf("Save complete!\n");

    return 0;
}
