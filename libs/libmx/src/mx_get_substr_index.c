#include "../inc/libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    if (!str || !sub) {
        return -2;
    }
    const char* char_ptr = mx_strstr(str, sub);
    if (!char_ptr) {
        return -1;
    }
    return char_ptr - str;  
}


