#include "../include/exec_lib.h"

/*  Gets the first digit of the user key
    1) Stampa
    2) Salva
    3) Invia
*/
int get_first_digit(int n) {
    int result = n;

    while(result >= 10) {
        result /= 10;
    }

    return result;
}

// Converts argv (from third to last) into a single string
char *args_to_string(char *args[]) {
    int length = args_size(args);
    char *args_string = (char *) malloc(length * sizeof(char) + 1);

    int i = 3;
    while(args[i] != NULL) {
        args_string = strcat(args_string, args[i]);

        if(args[i + 1] != NULL) {
            args_string = strcat(args_string, " ");
            i++;
        } else {
            i++;
        }
    }

    return args_string;
}

// Gets the size of argv (from third to last)
int args_size(char *args[]) {
    int s = 0;

    int i = 3;
    while(args[i] != NULL) {
        s += strlen(args[i]);
        i++;
    }

    return s;
}

Node_t *get_node(Node_t *shm_list, char *id, int user_key){
    Node_t *current = shm_list;

    while(current != shm_list + 20){
        if(strcmp(current -> id, id) == 0 && current -> user_key == user_key){
            return current;
        }
        ++current;
    }

    return NULL;

}