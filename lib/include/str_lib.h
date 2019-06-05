#ifndef STR_LIB_H
#define STR_LIB_H

#include <stddef.h>

void str_slice(const char *src, char *dest, size_t start, size_t end);
int validate_service(char service[]);
char *userkey_to_string(Response_t *user_key);

#endif