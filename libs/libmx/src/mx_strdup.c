#include "../inc/libmx.h"

char *mx_strdup(const char *s1) {
    if (!s1) {
        return NULL;
    }
    int new_size = mx_strlen(s1);
    char *res = mx_strnew(new_size);
    if (!res) {
        return NULL;
    }
    mx_strcpy(res, s1);
    return res;
}


