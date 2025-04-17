#include "../inc/libmx.h"

int mx_sqrt(int x) {
    if (x <= 0) {
        return 0;
    }
    if (x == 1) {
        return x;
    }
    int low = 0;
    int high = x;
    long mid;
    long sqr;
    while(low <= high) {
        mid = low + (high - low) / 2;
        sqr = mid * mid;
        if (sqr == x) {
            return mid;
        }
        else if (sqr < x) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return 0;
}


