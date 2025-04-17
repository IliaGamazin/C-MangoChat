#include "../inc/libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
    const unsigned char *src_temp = src;
    unsigned char *dst_temp = dst;
    if (src_temp < dst_temp){
        for (size_t i = len; i > 0; i--) {
            dst_temp[i - 1] = src_temp[i - 1];
        }
        return dst;
    }
    if (src_temp > dst_temp){
        mx_memcpy(dst, src, len);
		return dst;
    }
    return dst;
}


