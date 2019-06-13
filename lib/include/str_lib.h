#ifndef STR_LIB_H
#define STR_LIB_H

#include <stddef.h>
#include <ctype.h>

void str_slice(const char *src, char *dest, size_t start, size_t end);
void lower_case(char str[]);

#endif