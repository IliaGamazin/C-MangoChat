#include "../inc/libmx.h"

void mx_printint(int n) {
    long temp = n;
    if (n < 0) {
        mx_printchar('-');
        temp *= -1;
    }
    if (temp >= 10){
        mx_printint(temp / 10);
    }
    mx_printchar('0' + temp % 10);
}


