#include "../inc/contacts_groups.h"

void on_groups_button_clicked(GtkToggleButton *button, gpointer user_data) {
    (void)user_data;
    if (!gtk_toggle_button_get_active(button)) {
        return;
    }

    GtkWidget *add_new_contact_button = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "add_new_contact_button"));
    GtkWidget *add_new_group_button = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "add_new_group_button"));

    gtk_widget_set_visible(add_new_contact_button, FALSE);
    gtk_widget_set_visible(add_new_group_button, TRUE);
}

void on_member_execute_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;

    GtkWidget *member_box = user_data;
    GtkWidget *member_list_box = gtk_widget_get_parent(member_box);

    gtk_container_remove(GTK_CONTAINER(member_list_box), member_box);
}

void on_add_new_member_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;

    GtkPopover *popover = GTK_POPOVER(user_data);
    GtkWidget *member_list_box = g_object_get_data(G_OBJECT(popover), "member_list_box");
    GtkWidget *member_entry = g_object_get_data(G_OBJECT(popover), "member_entry");
    GtkStyleContext *context = gtk_widget_get_style_context(member_entry);

    const gchar *member_name = gtk_entry_get_text(GTK_ENTRY(member_entry));

    if (!strlen(member_name)) {
        return;
    }

    if (!username_validator(member_name)) {
        gtk_style_context_add_class(context, "error");
        return;
    }

    GList *members = retrieve_chat_members(member_list_box);
    if (has_dup_member(members, member_name)) {
        gtk_style_context_add_class(context, "error");
        g_list_free_full(members, g_free);
        gtk_entry_set_text(GTK_ENTRY(member_entry), "");
        return;
    } 

    gtk_style_context_remove_class(context, "error");

    GtkWidget *member_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(member_box, "member_box");

    GtkWidget *member_name_label = gtk_label_new(member_name);
    gtk_label_set_max_width_chars(GTK_LABEL(member_name_label), 24);
    gtk_label_set_ellipsize(GTK_LABEL(member_name_label), PANGO_ELLIPSIZE_END);
    gtk_widget_set_name(member_name_label, "member_name_label");

    GtkWidget *member_execute_button = gtk_button_new();
    gtk_widget_set_name(member_execute_button, "member_execute_button");

    gtk_widget_set_size_request(member_execute_button, 48, 24);
    g_object_set_data(G_OBJECT(member_box), "member_name_label", member_name_label);

    g_signal_connect(member_execute_button, "clicked", G_CALLBACK(on_member_execute_button_clicked), member_box);

    gtk_box_pack_start(GTK_BOX(member_box), member_execute_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(member_box), member_name_label, FALSE, FALSE, 0);

    gtk_list_box_insert(GTK_LIST_BOX(member_list_box), member_box, -1);
    gtk_widget_show_all(member_box);

    g_list_free_full(members, g_free);
    gtk_entry_set_text(GTK_ENTRY(member_entry), "");
}

void on_upload_group_avatar_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *parent_window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *dialog = create_filechooser_window(parent_window, true);
    GtkPopover *popover = GTK_POPOVER(user_data);
    gchar *selected_file = NULL;
    
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        selected_file = gtk_file_chooser_get_filename(chooser);
    }
    if (selected_file) {
        GdkPixbuf *rounded_pixbuf = create_rounded_pixbuf(selected_file, 42, 42);
        GtkWidget *avatar_image = g_object_get_data(G_OBJECT(button), "group_avatar_image");
        g_object_set_data_full(G_OBJECT(popover), "selected_file", selected_file, g_free);
        gtk_image_set_from_pixbuf(GTK_IMAGE(avatar_image), rounded_pixbuf);
        g_object_unref(rounded_pixbuf);
    }
    gtk_widget_destroy(dialog);
}

void on_add_group_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkPopover *popover = GTK_POPOVER(user_data);
    GtkWidget *member_list_box = g_object_get_data(G_OBJECT(popover), "member_list_box");
    GtkWidget *name_entry = g_object_get_data(G_OBJECT(popover), "group_name_entry");

    gchar *selected_file = g_object_get_data(G_OBJECT(popover), "selected_file");
    const gchar *chat_name = gtk_entry_get_text(GTK_ENTRY(name_entry));

    GList *member_list = retrieve_chat_members(member_list_box);
    member_list = g_list_append(member_list, g_strdup(main_data->username));
    cJSON *json = form_create_chat_request(chat_name, selected_file, member_list);    
    send_request(json, USER_CHAT_CREATE);
    g_list_free_full(member_list, g_free);

    GtkWidget *add_group_popover = GTK_WIDGET(user_data);
    gtk_popover_popdown(GTK_POPOVER(add_group_popover));
}

void on_add_new_group_button_clicked(GtkButton *button, gpointer user_data) {
    (void)user_data;

    GtkWidget *add_group_popover = gtk_popover_new(GTK_WIDGET(button));
    gtk_widget_set_name(add_group_popover, "add_group_popover");

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_name(main_box, "add_new_group_box");
    gtk_widget_set_size_request(main_box, 272, 150);

    GtkWidget *top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_name(top_box, "group_top_box");

    GtkWidget *group_avatar_image = gtk_image_new_from_file("img/group_avatar_placeholder.svg");
    GtkWidget *group_avatar_button = gtk_button_new();
    gtk_widget_set_name(group_avatar_button, "avatar_button");
    gtk_button_set_image(GTK_BUTTON(group_avatar_button), group_avatar_image);
    g_object_set_data(G_OBJECT(group_avatar_button), "group_avatar_image", group_avatar_image);
    g_signal_connect(group_avatar_button, "clicked", G_CALLBACK(on_upload_group_avatar_clicked), add_group_popover);

    GtkWidget *group_name_entry = gtk_entry_new();
    gtk_widget_set_name(group_name_entry, "group_name_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(group_name_entry), "Group name...");
    g_object_set_data(G_OBJECT(add_group_popover), "group_name_entry", group_name_entry);

    GtkWidget *add_group_button = gtk_button_new();
    gtk_widget_set_name(add_group_button, "add_group_button");
    GtkWidget *add_group_icon = gtk_image_new_from_file("img/add_group.svg");
    gtk_button_set_image(GTK_BUTTON(add_group_button), add_group_icon);
    g_signal_connect(add_group_button, "clicked", G_CALLBACK(on_add_group_button_clicked), add_group_popover);

    gtk_box_pack_start(GTK_BOX(top_box), group_avatar_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(top_box), group_name_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(top_box), add_group_button, FALSE, FALSE, 0);

    GtkWidget *member_entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(member_entry_box, "member_entry_box");

    GtkWidget *member_entry = gtk_entry_new();
    gtk_widget_set_name(member_entry, "group_member_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(member_entry), "Add member...");
    g_object_set_data(G_OBJECT(add_group_popover), "member_entry", member_entry);

    GtkWidget *add_new_member_button = gtk_button_new();
    gtk_widget_set_name(add_new_member_button, "add_new_member_button");
    GtkWidget *add_icon = gtk_image_new_from_file("img/add_person.svg");
    gtk_button_set_image(GTK_BUTTON(add_new_member_button), add_icon);
    g_signal_connect(add_new_member_button, "clicked", G_CALLBACK(on_add_new_member_button_clicked), add_group_popover);

    GtkWidget* member_list_box = gtk_list_box_new();
    gtk_widget_set_name(member_list_box, "member_box");
    g_object_set_data(G_OBJECT(add_group_popover), "member_list_box", member_list_box);

    gtk_box_pack_start(GTK_BOX(member_entry_box), member_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(member_entry_box), add_new_member_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(main_box), top_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), member_entry_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(main_box), member_list_box, FALSE, FALSE, 0);

    gtk_widget_set_size_request(group_name_entry, 100, 40);
    gtk_container_add(GTK_CONTAINER(add_group_popover), main_box);

    gtk_widget_show_all(add_group_popover);
    gtk_popover_popup(GTK_POPOVER(add_group_popover));
}


