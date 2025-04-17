#include "../inc/libmx.h"

char *mx_strcpy(char *dst, const char *src) {
    mx_memccpy(dst, src, '\0', mx_strlen(src) + 1);
    return dst;
}


