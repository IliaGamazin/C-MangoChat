#include "../inc/libmx.h"

char *mx_strstr(const char *haystack, const char *needle) {
    if (!haystack && !mx_strlen(needle)) {
        return NULL;
    } 
    return mx_memmem(haystack, mx_strlen(haystack), needle, mx_strlen(needle));
}


