#include "../inc/libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n) {
    unsigned char *dst_temp = dst;
    const unsigned char *src_temp = src;
    for (size_t i = 0; i < n; i++) {
        *dst_temp++ = *src_temp++;
    }
    return dst;
}


