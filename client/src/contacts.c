#include "../inc/contacts_groups.h"

void on_chats_button_clicked(GtkToggleButton *button, gpointer user_data) {
    (void)user_data;
    if (!gtk_toggle_button_get_active(button)) {
        return;
    }

    GtkWidget *add_new_contact_button = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "add_new_contact_button"));
    GtkWidget *add_new_group_button = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "add_new_group_button"));

    gtk_widget_set_visible(add_new_contact_button, TRUE);
    gtk_widget_set_visible(add_new_group_button, FALSE);
}

void on_add_contact_button_clicked(GtkButton *button, gpointer user_data) {
    (void)user_data;
    GList *member_list = NULL;
    GtkWidget *name_entry = g_object_get_data(G_OBJECT(button), "entry");
    GtkStyleContext *context = gtk_widget_get_style_context(name_entry);

    const gchar *contact_name = gtk_entry_get_text(GTK_ENTRY(name_entry));

    if (!strlen(contact_name)) {
        return;
    }

    if (!username_validator(contact_name)) {
        gtk_style_context_add_class(context, "error");
        return;
    }

    if (strings_match(main_data->username, contact_name)) {
        gtk_style_context_add_class(context, "error");
        gtk_entry_set_text(GTK_ENTRY(name_entry), "");
        return;
    } 
    
    gtk_style_context_remove_class(context, "error");

    member_list = g_list_append(member_list, g_strdup(main_data->username));
    member_list = g_list_append(member_list, g_strdup(contact_name));

    cJSON *json = form_create_chat_request(NULL, NULL, member_list);
    send_request(json, USER_CHAT_CREATE);
    g_list_free_full(member_list, g_free);

    GtkWidget *add_contact_popover = GTK_WIDGET(user_data);
    gtk_popover_popdown(GTK_POPOVER(add_contact_popover));
}

void on_add_new_contact_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;

    GtkWidget *add_contact_popover = gtk_popover_new(GTK_WIDGET(button));
    gtk_widget_set_name(add_contact_popover, "add_contact_popover");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_name(box, "add_new_contact_box");
    gtk_widget_set_size_request(box, 272, 100);

    GtkWidget *contact_name_entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *contact_name_entry = gtk_entry_new();
    gtk_widget_set_name(contact_name_entry, "contact_name_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(contact_name_entry), "Contact name...");

    GtkWidget *add_button = gtk_button_new();
    gtk_widget_set_name(add_button, "add_new_contact_button");
    GtkWidget *icon = gtk_image_new_from_file("img/add_person.svg");
    gtk_button_set_image(GTK_BUTTON(add_button), icon);

    gtk_box_pack_start(GTK_BOX(contact_name_entry_box), contact_name_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(contact_name_entry_box), add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), contact_name_entry_box, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(add_contact_popover), box);

    g_object_set_data(G_OBJECT(add_button), "entry", contact_name_entry);
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_contact_button_clicked), add_contact_popover);

    // issue
    gtk_widget_set_can_focus(contact_name_entry, FALSE);
    gtk_widget_show_all(add_contact_popover);
    gtk_widget_set_can_focus(contact_name_entry, TRUE);

    gtk_popover_popup(GTK_POPOVER(add_contact_popover));
}

