#include "../include/exec_lib.h"

int get_first_digit(int n) {
    int result = n;

    while(result >= 10) {
        result /= 10;
    }

    return result;
}