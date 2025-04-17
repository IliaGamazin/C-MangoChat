#include "../inc/headerbar.h"

void on_close_button_clicked(GtkWidget *widget, gpointer data) {
    (void) widget;
    gtk_widget_destroy(GTK_WIDGET(data));
}

void on_minimize_button_clicked(GtkWidget *widget, gpointer data) {
    (void) widget;
    gtk_window_iconify(GTK_WINDOW(data));
}

void on_fullscreen_button_clicked(GtkWidget *widget, gpointer data) {
    (void) widget;
    if (gtk_window_is_maximized(GTK_WINDOW(data))) {
        gtk_window_unmaximize(GTK_WINDOW(data));
        gtk_window_present(GTK_WINDOW(data));
        return;
    } 
    gtk_window_maximize(GTK_WINDOW(data));
}

void headbar_handler(GtkWidget* window, const char* png1, const char* png2, const char* png3, WINDOW_TYPE type) {
    GtkHeaderBar *header_bar = GTK_HEADER_BAR(gtk_header_bar_new());   
    GtkWidget *minimize_button = NULL;
    GtkWidget *close_button = NULL;
    GtkWidget *fullscreen_button = NULL;
    GtkWidget *minimize_image = NULL;
    GtkWidget *close_image = NULL;
    GtkWidget *fullscreen_image = NULL;

    minimize_button = gtk_button_new();
    close_button = gtk_button_new();
    minimize_image = gtk_image_new_from_file(png1);
    close_image = gtk_image_new_from_file(png2);
    
    gtk_header_bar_set_show_close_button(header_bar, FALSE); 

    gtk_widget_set_name(GTK_WIDGET(header_bar), "custom_header_bar");
    gtk_widget_set_name(GTK_WIDGET(minimize_button), "mangoMinimize_button");
    gtk_widget_set_name(GTK_WIDGET(close_button), "mangoClose_button");

    gtk_button_set_image(GTK_BUTTON(minimize_button), minimize_image);
    gtk_button_set_image(GTK_BUTTON(close_button), close_image);

    gtk_button_set_relief(GTK_BUTTON(minimize_button), GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(close_button), GTK_RELIEF_NONE);

    g_signal_connect(minimize_button, "clicked", G_CALLBACK(on_minimize_button_clicked), window);
    g_signal_connect(close_button, "clicked", G_CALLBACK(on_close_button_clicked), window);

    gtk_header_bar_pack_end(header_bar, close_button);

    if (type == MAIN) {
        fullscreen_button = gtk_button_new();
        fullscreen_image = gtk_image_new_from_file(png3);

        gtk_widget_set_name(GTK_WIDGET(fullscreen_button), "mangoFullscreen_button");
        gtk_button_set_image(GTK_BUTTON(fullscreen_button), fullscreen_image);
        gtk_button_set_relief(GTK_BUTTON(fullscreen_button), GTK_RELIEF_NONE);

        g_signal_connect(fullscreen_button, "clicked", G_CALLBACK(on_fullscreen_button_clicked), window);

        gtk_header_bar_pack_end(header_bar, fullscreen_button);
    }

    gtk_header_bar_pack_end(header_bar, minimize_button);
    gtk_window_set_titlebar(GTK_WINDOW(window), GTK_WIDGET(header_bar));
}

