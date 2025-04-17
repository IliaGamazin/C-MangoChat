#ifndef FILE_MANAGER
#define FILE_MANAGER

#include "../inc/libs.h"
#include "../inc/main_data.h"

unsigned char *read_file_to_buffer(const char *filename, long *size);
GtkWidget *create_filechooser_window(GtkWidget *parent_window, bool is_avatar_chooser);

#endif

