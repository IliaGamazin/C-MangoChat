#include "../inc/libmx.h"

char *mx_strjoin(char const *s1, char const *s2) {
    if (!s1 && !s2) {
        return NULL;
    }
    if (!s1) {
        char *dupl_str = mx_strdup(s2);
        return dupl_str;
    }
    if (!s2) {
        char *dupl_str = mx_strdup(s1);
        return dupl_str;
    }
    int s1_size = mx_strlen(s1);
    int s2_size = mx_strlen(s2);
    char *res = mx_strnew(s1_size + s2_size);
    if (!res) {
        return NULL;
    }
    mx_strcat(res, s1);
    mx_strcat(res, s2);
    return res;
}


