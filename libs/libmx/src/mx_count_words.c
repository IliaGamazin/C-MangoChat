#include "../inc/libmx.h"

int mx_count_words(const char *str, char c) {
    if (!str) {
        return -1;
    }
    bool in_word = false;
    int res = 0;
    while (*str) {
        if (*str == c) {
            in_word = false;
        } 
        else if (!in_word) {
            in_word = true;
            res++;
        }
        str++;
    }
    return res;
}


