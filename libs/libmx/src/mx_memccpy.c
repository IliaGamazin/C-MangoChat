#include "../inc/libmx.h"
#include <string.h>
void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
    unsigned char *dst_temp = dst;
    const unsigned char *src_temp = src;
    for (size_t i = 0; i < n; i++) {
        *dst_temp = *src_temp;
        if (*src_temp == (unsigned char)c) {
            return ++dst_temp;
        }
        dst_temp++;
        src_temp++;
    }
    return NULL;
}


