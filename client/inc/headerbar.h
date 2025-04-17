#ifndef HEADERBAR_H
#define HEADERBAR_H

#include "../inc/libs.h"

typedef enum {
    LOGIN,
    MAIN,
    MESSAGE
} WINDOW_TYPE;

void headbar_handler(GtkWidget* window, const char* png1, const char* png2, const char* png3, WINDOW_TYPE type);

#endif

