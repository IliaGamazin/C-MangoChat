#include "../inc/libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    if (!count) {
        return -1;
    }
    if (!arr || !s) {
        *count = 0;
        return -1; 
    }
    int low = 0;
    int high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        (*count)++; 
        if (!mx_strcmp(arr[mid], s)) {
            return mid;
        }
        else if (mx_strcmp(arr[mid], s) < 0) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    *count = 0;
    return -1; 
}


