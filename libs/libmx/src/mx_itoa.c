#include "../inc/libmx.h"

char *mx_itoa(int number) {
    bool is_negative = number < 0;
    int len = is_negative;
    long temp_counter = number;
    long temp = number;
    if (is_negative){
        temp *= -1;
        temp_counter *= -1;
    }
    if (!number) {
        len++;
    }
    while(temp_counter > 0) {
        len++;
        temp_counter /= 10;
    }
    char *res = mx_strnew(len);
    if (is_negative){
        res[0] = '-';
    }
    for (int i = is_negative; i < len; i++) {
        res[i] = '0' + temp / (int)mx_pow(10, len - i - 1) % 10;
    }
    return res;
}


