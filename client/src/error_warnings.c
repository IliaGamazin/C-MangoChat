#include "../inc/error_warnings.h"
#include "../../utils/inc/request_status.h"

gboolean update_notification_label_register(gpointer user_data) {
    (void)user_data;
    int status = GPOINTER_TO_INT(user_data);

    GtkWidget *login_window = g_object_get_data(G_OBJECT(main_data->buffer), "login-window");
    GtkWidget *notification_label = g_object_get_data(G_OBJECT(login_window), "notification_label");

    const gchar *message = NULL;
    const gchar *css_class = "error"; 

    switch (status) {
        case RQ_SUCCESS:
            message = "Signed up successfully!";
            css_class = "success"; 
            break;
        case RQ_JSON_INVALID:
            message = "Skill issue json edition!";
            break; 
        case RQ_USER_EXISTS:
            message = "User already exists!";
            break; 
        case RQ_USER_INVALID:
            message = "Wow! No way you got this!";
            break;
        default:
            message = "Unknown error occurred!";
            break; 
    }

    gtk_label_set_text(GTK_LABEL(notification_label), message);

    GtkStyleContext *context = gtk_widget_get_style_context(notification_label);
    gtk_style_context_remove_class(context, "success");
    gtk_style_context_remove_class(context, "error");
    gtk_style_context_add_class(context, css_class);

    gtk_widget_show(notification_label);

    return G_SOURCE_REMOVE;
}

gboolean update_notification_label_login(gpointer user_data) {
    (void)user_data;
    int status = GPOINTER_TO_INT(user_data);

    GtkWidget *login_window = g_object_get_data(G_OBJECT(main_data->buffer), "login-window");
    GtkWidget *notification_label = g_object_get_data(G_OBJECT(login_window), "notification_label");

    const gchar *message = "Invalid username or password!";
    const gchar *css_class = "error"; 


    gtk_label_set_text(GTK_LABEL(notification_label), message);

    GtkStyleContext *context = gtk_widget_get_style_context(notification_label);
    gtk_style_context_add_class(context, css_class);

    if (status == RQ_USER_INVALID) {
        gtk_widget_show(notification_label);
    }

    return G_SOURCE_REMOVE;
}

gboolean change_username_error(gpointer user_data) {
    (void)user_data;

    GtkWidget *new_username_entry = g_object_get_data(G_OBJECT(main_data->buffer), "new_username_entry");    
    GtkWidget *confirm_new_username_entry = g_object_get_data(G_OBJECT(main_data->buffer), "confirm_new_username_entry");

    GtkStyleContext *first_entry_context = gtk_widget_get_style_context(new_username_entry);
    GtkStyleContext *second_entry_context = gtk_widget_get_style_context(confirm_new_username_entry);

    gtk_style_context_add_class(first_entry_context, "error");
    gtk_style_context_add_class(second_entry_context, "error");

    return G_SOURCE_REMOVE;
}

gboolean change_password_error(gpointer user_data) {
    (void)user_data;

    GtkWidget *new_password_entry = g_object_get_data(G_OBJECT(main_data->buffer), "new_password_entry");    
    GtkWidget *confirm_new_password_entry = g_object_get_data(G_OBJECT(main_data->buffer), "confirm_new_password_entry");

    GtkStyleContext *first_entry_context = gtk_widget_get_style_context(new_password_entry);
    GtkStyleContext *second_entry_context = gtk_widget_get_style_context(confirm_new_password_entry);

    gtk_style_context_add_class(first_entry_context, "error");
    gtk_style_context_add_class(second_entry_context, "error");

    return G_SOURCE_REMOVE;
}
