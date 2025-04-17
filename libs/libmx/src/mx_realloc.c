#include "../inc/libmx.h"

void *mx_realloc(void *ptr, size_t size) {
    if (!size && ptr) {
        free(ptr);
        return NULL;
    }
    if (!ptr) {
        return malloc(size);
    }
    void *res = NULL;
    if (!(res = malloc(size))){
        return NULL;
    }
    size_t allocated_size = malloc_usable_size(ptr);
    size_t copy_size = (size < allocated_size) ? size : allocated_size;
    mx_memcpy(res, ptr, copy_size);
    free(ptr);
    return res;
}



