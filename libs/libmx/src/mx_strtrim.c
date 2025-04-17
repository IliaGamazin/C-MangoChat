#include "../inc/libmx.h"

char *mx_strtrim(const char *str) {
    if (!str) {
        return NULL;
    }
    int start = 0;
    int end = mx_strlen(str) - 1;
    while (mx_isspace(str[start])) {
        start++;
    }
    if (str[start] == '\0') {
        return mx_strnew(0); 
    }
    while (mx_isspace(str[end])) {
        end--;
    }
    char *res = mx_strnew(end - start + 1);
    if (!res) {
        return NULL;
    }
    mx_strncpy(res, &str[start], end - start + 1);
    return res;
}


