#include "../inc/libmx.h"

char *mx_file_to_str(const char *file) {
    if (!file) {
    	return NULL;
    }
    int fd;
    if ((fd = open(file, O_RDONLY)) < 0) {
    	return NULL;
    }
    int len = 0;
    char buffer;
	while (read(fd, &buffer, 1)) {
		len++;
	}
    if (close(fd) < 0) {
        return NULL;
    }
    int fd1;
    if ((fd1 = open(file, O_RDONLY)) < 0) {
        return NULL;
    }
    char *res = mx_strnew(len);
    if (!res) {
        return NULL;
    }
    for (int i = 0; read(fd1, &buffer, 1); i++) {
        res[i] = buffer;
    }
    if (close(fd) < 0) {
        free(res);
        return NULL;
    }
    return res;
}


