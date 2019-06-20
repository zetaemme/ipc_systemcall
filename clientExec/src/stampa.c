#include <stdlib.h>
#include <stdio.h>

#include "../../lib/include/err_lib.h"

int main (int argc, char *argv[]) {
    int i = 1;

    if(argc < 2) {
        err_exit("<Print> Not enough argunents");
    }

    // Prints the string
    while(argv[i] != NULL) {
        printf("%s ", argv[i]);
        i++;
    }

    printf("\n");

    return 0;
}
