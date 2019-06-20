#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../../lib/include/err_lib.h"

int main(int argc, char *argv[]) { 
    // Generate the new file's filename
    char filename[256]; 
    sprintf(filename, "%s.txt", argv[1]);

    if(argc < 3){
    	err_exit("<Save> Not enough arguments!");
    }

    printf("Creating file...\t\t");

    // Creates exclusively the new file
    int file = open(filename, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);

    if(file == -1) {
        err_exit("\n<Save> open failed");
    } else {
        printf("DONE!\n");
    }

    // Buffer that contains the string and while loop index
    char buffer[256];
    int i = 2;

    // Write args into file 
    strcpy(buffer, argv[i]);
    ssize_t wB = write(file, buffer, sizeof(char) * strlen(argv[i]));

    if(wB != sizeof(char) * strlen(argv[i])) {
        err_exit("<Save> write failed");
    }

    printf("Save complete!\n");

    return 0;
}
