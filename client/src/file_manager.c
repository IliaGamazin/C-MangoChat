#include "../inc/file_manager.h"

unsigned char *read_file_to_buffer(const char *filename, long *size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(*size);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *size, file);
    fclose(file);
    return buffer;
}

GtkWidget *create_filechooser_window(GtkWidget *parent_window, bool is_avatar_chooser) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                    GTK_WINDOW(parent_window),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);
    gtk_widget_set_size_request(dialog, 800, 800);
    gtk_widget_set_name(dialog, "dialog");
    if (is_avatar_chooser) {
        GtkFileFilter *file_filter  = gtk_file_filter_new();
        gtk_file_filter_set_name (file_filter,"Images");
        gtk_file_filter_add_pattern (file_filter, "*.png");                                 
        gtk_file_filter_add_pattern (file_filter, "*.jpg");                                 
        gtk_file_filter_add_pattern (file_filter, "*.jpeg");
        gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(dialog),file_filter);  
    }

    return dialog;
}



