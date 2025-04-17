#include "../inc/libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *s1_temp = s1;
    const unsigned char *s2_temp = s2;
    for (size_t i = 0; i < n; i++) {
        if (*s1_temp != *s2_temp) {
            return *s1_temp - *s2_temp;
        }
        s1_temp++;
        s2_temp++;
    }
    return 0;
}



