#include "../inc/libmx.h"

void mx_str_reverse(char *s) {
    if (!s) {
        return;
    }
    
    int len = mx_strlen(s);
    int l_index = 0;
    int r_index = len - 1;
    while(r_index > l_index) {
        mx_swap_char(&s[l_index], &s[r_index]);
        l_index++;
        r_index--;
    }
}


