#include "../inc/libmx.h"

char *mx_strndup(const char *s1, size_t n) {
    if (!s1 && !n) {
        return mx_strnew(0);
    }
    size_t s1_size = mx_strlen(s1);
    size_t res_size = s1_size > n ? n : s1_size;
    char *res = mx_strnew(res_size);
    if (!res) {
        return NULL;
    }
    mx_strncpy(res, s1, res_size);
    return res;

}


