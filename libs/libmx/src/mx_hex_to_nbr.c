#include "../inc/libmx.h"

unsigned long mx_hex_to_nbr(const char *hex) {
    if (!hex) {
        return 0;
    }
    unsigned long res = 0;
    while (*hex) {
        res *= 16;
        if (*hex >= '0' && *hex <= '9') {
            res += *hex - '0';
        }
        else if (*hex >= 'A' && *hex <= 'F') {
            res += *hex - 'A' + 10;
        }
        else if (*hex >= 'a' && *hex <= 'f') {
            res += *hex - 'a' + 10;
        }
        else {
            return 0;
        }
        hex++;
    }
    return res;
}


