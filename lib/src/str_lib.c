#include "../include/str_lib.h"

// Slices src string from start to end and saves it into dest
void str_slice(const char *src, char *dest, size_t start, size_t end) {
    size_t j = 0;

    for (size_t i = start; i <= end; ++i) {
        dest[j++] = src[i];
    }
}