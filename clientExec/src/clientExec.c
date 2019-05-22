#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    char *id, user_key, args;

    id = argv[1];
    user_key = argv[2];
    args = argv[3];

    // Accesso a segmento di Shared Memory e validazione dati (TODO)

    // Esegui il servizio richiesto tramite syscall exec

    // Finire!!

    return 0;
}
