#include "../inc/libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    if (!str || !sub) {
        return -1;
    }
    if (!strlen(sub)) {
        return 0;
    }
    int res = 0;
    while ((str = mx_strstr(str, sub)) != NULL) {
        res++;
        str++;
    }
    return res;
}


