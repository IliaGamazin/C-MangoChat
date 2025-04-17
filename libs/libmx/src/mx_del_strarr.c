#include "../inc/libmx.h"

void mx_del_strarr(char ***str) {
    if (!(*str) || !str) {
        return;
    }
    for (int i = 0; (*str)[i]; i++) {
        mx_strdel(&((*str)[i]));
    }
    free(*str);
    *str = NULL;
}


