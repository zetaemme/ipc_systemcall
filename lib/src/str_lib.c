#include "../include/str_lib.h"

// Turns the string lowercase
void lower_case(char str[]) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}