#include "../inc/libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
    if (!nbr) {
        char *res = mx_strnew(1);
        if (!res) {
            return NULL;  
        }
        res[0] = '0';
        return res;
    }
    int len = 0;
    unsigned long temp = nbr;
    while (temp) {
        temp /= 16;
        len++;
    }
    char *res = mx_strnew(len);
    if (!res) {
        return NULL;  
    }
    for (int i = len - 1; i >= 0; i--) {
        int remainder = nbr % 16;
        if (remainder < 10) {
            res[i] = '0' + remainder;
        } 
        else {
            res[i] = 'a' + remainder - 10;
        }
        nbr /= 16;
    }
    return res;
}


