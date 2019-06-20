#ifndef EXEC_LIB_H
#define EXEC_LIB_H

#include <stdlib.h>
#include <string.h>

#include "./list_lib.h"

int get_first_digit(int n);
char *args_to_string(char *array[]);
int args_size(char *array[]);
Node_t *get_node(Node_t *shm_list, char *id, int user_key);

#endif