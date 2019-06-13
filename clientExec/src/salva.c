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
        printf("\033[1;31m");
        errExit("\n<Exec> open failed");
        printf("\033[0m");
    } else {
        printf("\033[1;32m");
        printf("DONE!\n");
        printf("\033[0m");
    }

    // Buffer that contains the string and while loop index
    char buffer[256];
    int i = 3;

    while(argv[i] != NULL) {
        strcpy(buffer, argv[i]);

        if(write(file, buffer, sizeof(argv[i]))) {
            printf("\033[1;31m");
            errExit("<Exec> write failed");
            printf("\033[0m");
        }

        i++;
    }

    printf("\033[1;32m");
    printf("Save complete!\n");
    printf("\033[0m");

    return 0;
}
