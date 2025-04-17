#include "../inc/settings.h"

gboolean on_avatar_received(gpointer user_data) {
    (void)user_data;
    GtkWidget *parent_window = g_object_get_data(G_OBJECT(main_data->buffer), "main_window");
    GtkWidget *avatar_image = g_object_get_data(G_OBJECT(parent_window), "avatar_image");
    gtk_image_set_from_pixbuf(GTK_IMAGE(avatar_image), main_data->avatar);

    return G_SOURCE_REMOVE;
}

void on_avatar_settings_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);
    gtk_widget_set_name(main_window, "main_window");
    GtkWidget *settings_popover = gtk_popover_new(GTK_WIDGET(button));

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_name(box, "settings_popover_box");
    gtk_widget_set_size_request(box, 264, 100);

    GtkWidget *change_avatar_button = gtk_button_new_with_label("Change avatar");
    gtk_widget_set_halign(change_avatar_button, GTK_ALIGN_START);
    gtk_widget_set_name(change_avatar_button, "settings_button");
    gtk_box_pack_start(GTK_BOX(box), change_avatar_button, FALSE, FALSE, 0);
    g_signal_connect(change_avatar_button, "clicked", G_CALLBACK(on_change_avatar_button_clicked), main_window);

    GtkWidget *change_username_button = gtk_button_new_with_label("Change username");
    gtk_widget_set_halign(change_username_button, GTK_ALIGN_START);
    gtk_widget_set_name(change_username_button, "settings_button");
    gtk_box_pack_start(GTK_BOX(box), change_username_button, FALSE, FALSE, 0);
    g_signal_connect(change_username_button, "clicked", G_CALLBACK(on_change_username_button_clicked), settings_popover);
    g_object_set_data(G_OBJECT(settings_popover), "username_box", NULL);

    GtkWidget *change_password_button = gtk_button_new_with_label("Change password");
    gtk_widget_set_halign(change_password_button, GTK_ALIGN_START);
    gtk_widget_set_name(change_password_button, "settings_button");
    gtk_box_pack_start(GTK_BOX(box), change_password_button, FALSE, FALSE, 0);
    g_signal_connect(change_password_button, "clicked", G_CALLBACK(on_change_password_button_clicked), settings_popover);
    g_object_set_data(G_OBJECT(settings_popover), "password_box", NULL);

    GtkWidget *logout_button = gtk_button_new_with_label("Log out");
    gtk_widget_set_halign(logout_button, GTK_ALIGN_START);
    gtk_widget_set_name(logout_button, "logout_button");
    gtk_box_pack_start(GTK_BOX(box), logout_button, FALSE, FALSE, 0);
    g_signal_connect(logout_button, "clicked", G_CALLBACK(on_logout_button_clicked), main_window);

    gtk_container_add(GTK_CONTAINER(settings_popover), box);
    
    gtk_widget_show_all(settings_popover);
    gtk_popover_popup(GTK_POPOVER(settings_popover));
}

void on_change_avatar_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget *parent_window = GTK_WIDGET(user_data);
    GtkWidget *dialog = create_filechooser_window(parent_window, true);
    gchar *selected_file = NULL;
    
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        selected_file = gtk_file_chooser_get_filename(chooser);
    }

    if (selected_file) {
        cJSON *json = form_change_avatar_request(selected_file);
        send_request(json, USER_SET_AVATAR);
        GdkPixbuf *rounded_pixbuf = create_rounded_pixbuf(selected_file, 42, 42);
        GtkWidget *avatar_image = g_object_get_data(G_OBJECT(parent_window), "avatar_image");
        g_free(selected_file);
        gtk_image_set_from_pixbuf(GTK_IMAGE(avatar_image), rounded_pixbuf);
        g_object_unref(rounded_pixbuf);
    }
    
    gtk_widget_destroy(dialog);
}

void on_change_username_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget *popover = GTK_WIDGET(user_data);

    GtkWidget *username_box = g_object_get_data(G_OBJECT(popover), "username_box");
    GtkWidget *password_box = g_object_get_data(G_OBJECT(popover), "password_box");

    if (password_box && gtk_widget_get_visible(password_box)) {
        GtkWidget *new_username_entry = g_object_get_data(G_OBJECT(main_data->buffer), "new_username_entry");
        GtkWidget *confirm_new_username_entry = g_object_get_data(G_OBJECT(main_data->buffer), "confirm_new_username_entry");

        if (new_username_entry) gtk_entry_set_text(GTK_ENTRY(new_username_entry), "");  
        if (confirm_new_username_entry) gtk_entry_set_text(GTK_ENTRY(confirm_new_username_entry), "");  

        gtk_widget_hide(password_box);
    }

    if (username_box && gtk_widget_get_visible(username_box)) {
        GtkWidget *new_username_entry = g_object_get_data(G_OBJECT(main_data->buffer), "new_username_entry");
        GtkWidget *confirm_new_username_entry = g_object_get_data(G_OBJECT(main_data->buffer), "confirm_new_username_entry");

        if (new_username_entry) gtk_entry_set_text(GTK_ENTRY(new_username_entry), "");  
        if (confirm_new_username_entry) gtk_entry_set_text(GTK_ENTRY(confirm_new_username_entry), ""); 
         
        gtk_widget_hide(username_box);
        return;
    }

    if (!username_box) {
        username_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_widget_set_name(username_box, "username_box");

        GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_widget_set_name(separator, "separator");
        gtk_widget_set_halign(separator, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(separator, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(username_box), separator, FALSE, FALSE, 0);

        GtkWidget *new_username_entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(new_username_entry), "New username");
        gtk_widget_set_name(new_username_entry, "new_username_entry");
        gtk_widget_set_margin_top(new_username_entry, 5);
        gtk_box_pack_start(GTK_BOX(username_box), new_username_entry, FALSE, FALSE, 5);

        GtkWidget *confirm_new_username_entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_new_username_entry), "Confirm username");
        gtk_widget_set_name(confirm_new_username_entry, "confirm_new_username_entry");
        gtk_box_pack_start(GTK_BOX(username_box), confirm_new_username_entry, FALSE, FALSE, 5);

        GtkWidget *update_username_button = gtk_button_new_with_label("Update username");
        gtk_widget_set_name(update_username_button, "update_username_button");
        gtk_box_pack_start(GTK_BOX(username_box), update_username_button, FALSE, FALSE, 5);

        g_signal_connect(update_username_button, "clicked", G_CALLBACK(on_save_username_clicked), popover);

        g_object_set_data(G_OBJECT(main_data->buffer), "new_username_entry", new_username_entry);
        g_object_set_data(G_OBJECT(main_data->buffer), "confirm_new_username_entry", confirm_new_username_entry);

        GtkWidget *box = gtk_bin_get_child(GTK_BIN(popover));
        gtk_box_pack_start(GTK_BOX(box), username_box, FALSE, FALSE, 5);

        g_object_set_data(G_OBJECT(popover), "username_box", username_box);
    }

    gtk_widget_show_all(username_box);
}

void on_save_username_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;

    GtkWidget *new_username_entry = g_object_get_data(G_OBJECT(main_data->buffer), "new_username_entry");    
    GtkWidget *confirm_new_username_entry = g_object_get_data(G_OBJECT(main_data->buffer), "confirm_new_username_entry");

    GtkStyleContext *first_entry_context = gtk_widget_get_style_context(new_username_entry);
    GtkStyleContext *second_entry_context = gtk_widget_get_style_context(confirm_new_username_entry);

    gtk_style_context_remove_class(first_entry_context, "error");
    gtk_style_context_remove_class(second_entry_context, "error");

    const gchar *new_username = gtk_entry_get_text(GTK_ENTRY(new_username_entry));
    const gchar *confirm_new_username = gtk_entry_get_text(GTK_ENTRY(confirm_new_username_entry));

    if (!username_validator(new_username)) {
        gtk_style_context_add_class(first_entry_context, "error");
        gtk_style_context_add_class(second_entry_context, "error");
        fprintf(stderr, "[USERNAME CHANGE] Invalid username\n");

        return;
    }

    if (!strings_match(new_username, confirm_new_username)) {
        gtk_style_context_add_class(first_entry_context, "error");
        gtk_style_context_add_class(second_entry_context, "error");
        fprintf(stderr, "[USERNAME CHANGE] Usernames do not match\n");
        
        return;
    }

    if (strings_match(main_data->username, new_username)) {
        gtk_style_context_add_class(first_entry_context, "error");
        gtk_style_context_add_class(second_entry_context, "error");
        fprintf(stderr, "[USERNAME CHANGE] Username similar to the current one\n");
        
        return;
    }

    cJSON *json = form_change_username_request(new_username);
    send_request(json, USER_SET_USERNAME);
}

void on_change_password_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget *popover = GTK_WIDGET(user_data);

    GtkWidget *password_box = g_object_get_data(G_OBJECT(popover), "password_box");
    GtkWidget *username_box = g_object_get_data(G_OBJECT(popover), "username_box");

    if (username_box && gtk_widget_get_visible(username_box)) {
        GtkWidget *new_password_entry = g_object_get_data(G_OBJECT(main_data->buffer), "new_password_entry");
        GtkWidget *confirm_new_password_entry = g_object_get_data(G_OBJECT(main_data->buffer), "confirm_new_password_entry");

        if (new_password_entry) gtk_entry_set_text(GTK_ENTRY(new_password_entry), "");  
        if (confirm_new_password_entry) gtk_entry_set_text(GTK_ENTRY(confirm_new_password_entry), "");  

        gtk_widget_hide(username_box);
    }

    if (password_box && gtk_widget_get_visible(password_box)) {
        GtkWidget *new_password_entry = g_object_get_data(G_OBJECT(main_data->buffer), "new_password_entry");
        GtkWidget *confirm_new_password_entry = g_object_get_data(G_OBJECT(main_data->buffer), "confirm_new_password_entry");

        if (new_password_entry) gtk_entry_set_text(GTK_ENTRY(new_password_entry), "");  
        if (confirm_new_password_entry) gtk_entry_set_text(GTK_ENTRY(confirm_new_password_entry), ""); 

        gtk_widget_hide(password_box);
        return;
    }

    if (!password_box) {
        password_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_widget_set_name(password_box, "password_box");

        GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_widget_set_name(separator, "separator");
        gtk_widget_set_halign(separator, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(separator, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(password_box), separator, FALSE, FALSE, 0);

        GtkWidget *new_password_entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(new_password_entry), "New password");
        gtk_entry_set_visibility(GTK_ENTRY(new_password_entry), FALSE);
        gtk_entry_set_icon_from_icon_name(GTK_ENTRY(new_password_entry), GTK_ENTRY_ICON_SECONDARY, "view-reveal-symbolic");
        gtk_widget_set_name(new_password_entry, "new_password_entry");
        gtk_widget_set_margin_top(new_password_entry, 5);
        gtk_box_pack_start(GTK_BOX(password_box), new_password_entry, FALSE, FALSE, 5);

        g_signal_connect(new_password_entry, "icon-press", G_CALLBACK(toggle_password_visibility_icon), NULL);

        GtkWidget *confirm_new_password_entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_new_password_entry), "Confirm password");
        gtk_entry_set_visibility(GTK_ENTRY(confirm_new_password_entry), FALSE);
        gtk_entry_set_icon_from_icon_name(GTK_ENTRY(confirm_new_password_entry), GTK_ENTRY_ICON_SECONDARY, "view-reveal-symbolic");
        gtk_widget_set_name(confirm_new_password_entry, "confirm_new_password_entry");
        gtk_box_pack_start(GTK_BOX(password_box), confirm_new_password_entry, FALSE, FALSE, 5);

        g_signal_connect(confirm_new_password_entry, "icon-press", G_CALLBACK(toggle_password_visibility_icon), NULL);

        GtkWidget *update_password_button = gtk_button_new_with_label("Update password");
        gtk_widget_set_name(update_password_button, "update_password_button");
        gtk_box_pack_start(GTK_BOX(password_box), update_password_button, FALSE, FALSE, 5);

        g_signal_connect(update_password_button, "clicked", G_CALLBACK(on_save_password_clicked), popover);

        g_object_set_data(G_OBJECT(main_data->buffer), "new_password_entry", new_password_entry);
        g_object_set_data(G_OBJECT(main_data->buffer), "confirm_new_password_entry", confirm_new_password_entry);

        GtkWidget *box = gtk_bin_get_child(GTK_BIN(popover));
        gtk_box_pack_start(GTK_BOX(box), password_box, FALSE, FALSE, 5);

        g_object_set_data(G_OBJECT(popover), "password_box", password_box);
    }

    gtk_widget_show_all(password_box);
}

void on_save_password_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;

    GtkWidget *new_password_entry = g_object_get_data(G_OBJECT(main_data->buffer), "new_password_entry");    
    GtkWidget *confirm_new_password_entry = g_object_get_data(G_OBJECT(main_data->buffer), "confirm_new_password_entry");

    GtkStyleContext *first_entry_context = gtk_widget_get_style_context(new_password_entry);
    GtkStyleContext *second_entry_context = gtk_widget_get_style_context(confirm_new_password_entry);

    const gchar *new_password = gtk_entry_get_text(GTK_ENTRY(new_password_entry));
    const gchar *confirm_new_password = gtk_entry_get_text(GTK_ENTRY(confirm_new_password_entry));

    gtk_style_context_remove_class(first_entry_context, "error");
    gtk_style_context_remove_class(second_entry_context, "error");

    if (!password_validator(new_password)) {
        gtk_style_context_add_class(first_entry_context, "error");
        gtk_style_context_add_class(second_entry_context, "error");
        fprintf(stderr, "[USERNAME CHANGE] Invalid password\n");

        return;
    }

    if (!strings_match(new_password, confirm_new_password)) {
        gtk_style_context_add_class(first_entry_context, "error");
        gtk_style_context_add_class(second_entry_context, "error");
        fprintf(stderr, "[PASSWORD CHANGE] Passwords do not match\n");

        return;
    }
    
    cJSON *json = form_change_password_request(new_password);
    send_request(json, USER_SET_PASSWORD);
}

gboolean remove_all_callback(gpointer key, gpointer value, gpointer user_data) {
    (void)key; (void)value; (void)user_data;
    return TRUE;
}

void on_logout_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget *current_window = GTK_WIDGET(user_data);

    if (GTK_IS_WIDGET(current_window)) {
        gtk_widget_destroy(current_window);
    }

    if (main_data->username) {
        free(main_data->username);
    }
    if (main_data->message_order) {
        g_list_free(main_data->message_order);
    }

    main_data->current_chat_id = -1;

    g_hash_table_foreach_remove(main_data->users_cache, remove_all_callback, NULL);
    g_hash_table_foreach_remove(main_data->chats_cache, remove_all_callback, NULL);
    g_hash_table_foreach_remove(main_data->messages_cache, remove_all_callback, NULL);

    main_data->message_order = NULL;
    

    create_login_window(NULL);
}

