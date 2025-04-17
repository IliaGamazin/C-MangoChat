#ifndef AVATAR_HANDLER_H
#define AVATAR_HANDLER_H

#include "../inc/libs.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

GdkPixbuf* create_rounded_pixbuf(gchar *selected_file, int dest_width, int dest_height);
GdkPixbuf* create_rounded_pixbuf_from_memory(const guchar *file_data, gsize data_length, int dest_width, int dest_height);

#endif

