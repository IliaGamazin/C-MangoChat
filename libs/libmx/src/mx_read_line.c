#include "../inc/libmx.h"

void mx_join_buffers(char** buf, char** src) {
    char *temp_buf = *buf;
    *buf = mx_strjoin(*buf, *src);
    mx_strdel(&temp_buf);
}

void mx_clean(char **buf, char **big_buf) {
    mx_strdel(buf);
    mx_strdel(big_buf);
}

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    if (fd < 0 || !lineptr) {
        return -2;
    }
    static char *leftover = NULL;
    char *buf = mx_strnew(buf_size);
    char *big_buf = leftover ? mx_strdup(leftover) : mx_strnew(0);
    mx_strdel(&leftover);
    ssize_t bytes_read = 0;
    while ((bytes_read = read(fd, buf, buf_size)) > 0) {
        buf[bytes_read] = '\0';
        mx_join_buffers(&big_buf, &buf);
        int del_index = mx_get_char_index(big_buf, delim);
        if (del_index != -1) {
            if (*lineptr) {
                mx_strdel(lineptr);
            }
            *lineptr = mx_strndup(big_buf, del_index);
            leftover = mx_strdup(big_buf + del_index + 1);
            mx_clean(&buf, &big_buf);
            return mx_strlen(*lineptr);
        }
    }
    mx_clean(&buf, &big_buf);
    mx_strdel(&leftover);
    return -1;
}


