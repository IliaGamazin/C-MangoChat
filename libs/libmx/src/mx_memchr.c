#include "../inc/libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    const unsigned char *src_temp = s;
    for (size_t i = 0; i < n; i++) {
        if (*src_temp == (unsigned char)c) {
            return (void *)src_temp;
        }
        src_temp++;
    }
    return NULL;
}


