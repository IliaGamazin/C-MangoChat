#include "../inc/client.h"

void import_css_theme(const char* theme) {
    GtkCssProvider *css_provider = gtk_css_provider_new();
    GFile *css_file = g_file_new_for_path(theme);
    gtk_css_provider_load_from_file(css_provider, css_file, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(css_provider);
    g_object_unref(css_file);
}
