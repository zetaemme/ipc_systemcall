#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    int i = 3;

    while(argv[i] != NULL) {
        printf("%s ", argv[i]);
    }

    return 0;
}
