#include "../inc/libmx.h"

void mx_print_unicode(wchar_t c) {
    char res[4];
    int len = 0;
    int prefix = 0x00;
    if (c <= 0x7F) {
        mx_printchar(c);
        return;
    }
    else if (c <= 0x7FF) {
        len = 2;
        prefix = 0xC0;
    }
    else if (c <= 0xFFFF) {
        len = 3;
        prefix = 0xE0;
    }
    else if (c <= 0x10FFFF) {
        len = 4;
        prefix = 0xF0;
    }
    res[0] = (c >> (6 * (len - 1))) | prefix;
    for (int i = 1; i < len; i++) {
        res[i] = ((c >> (6 * (len - i - 1))) & 0x3F) | 0x80;
    }
    write(STDOUT_FILENO, res, len);
}


