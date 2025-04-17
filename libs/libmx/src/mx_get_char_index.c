#include "../inc/libmx.h"

int mx_get_char_index(const char *str, char c) {
    if (!str) {
        return -2;
    }
    const char* char_ptr = mx_memchr(str, c, mx_strlen(str));
    if (!char_ptr) {
        return -1; 
    }
    return char_ptr - str;
}


