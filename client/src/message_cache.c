#include "../inc/session_cache.h"

static void on_edit_chat_entry_activate(GtkButton *entry, gpointer user_data) {
    Message *message_data = (Message *)user_data;
    int message_id = message_data->message_id;
    g_print("Message ID: %d\n", message_id);

    GtkWidget *chat_entry_box= g_object_get_data(G_OBJECT(main_data->buffer), "chat_entry_box");
    GtkWidget *chat_entry = g_object_get_data(G_OBJECT(chat_entry_box), "chat_entry");

    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry)); 
    g_print("Entered text: %s\n", text); 

    cJSON *json = form_edit_message_request(message_id, (char*)text);
    send_request(json, USER_MESSAGE_EDIT); // When rt type will update change

    gtk_widget_destroy(GTK_WIDGET(entry));
    gtk_widget_show(chat_entry);
}

static void on_edit_message_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    Message *message_data = (Message *)user_data;
    int message_id = message_data->message_id;
    g_print("Message ID: %d\n", message_id);

    GtkWidget *chat_entry_box = g_object_get_data(G_OBJECT(main_data->buffer), "chat_entry_box");
    GtkWidget *chat_entry = g_object_get_data(G_OBJECT(chat_entry_box), "chat_entry");

    GList *children = gtk_container_get_children(GTK_CONTAINER(chat_entry_box));
    int count = g_list_length(children);
    g_list_free(children);
    
    if (count > 3) {
        return;
    }

    GtkWidget *edit_chat_entry = gtk_entry_new();
    gtk_widget_set_name(edit_chat_entry, "edit_chat_entry");
    gtk_widget_set_size_request(edit_chat_entry, 492, 48);
    gtk_box_pack_start(GTK_BOX(chat_entry_box), edit_chat_entry, FALSE, FALSE, 0);
    gtk_entry_set_text(GTK_ENTRY(edit_chat_entry), message_data->text);

    g_signal_connect(edit_chat_entry, "activate", G_CALLBACK(on_edit_chat_entry_activate), message_data);

    gtk_widget_show_all(chat_entry_box);
    gtk_widget_hide(chat_entry);
}

gboolean on_hover_notify(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
    (void)user_data;
    GtkWidget *edit_button = g_object_get_data(G_OBJECT(widget), "edit_message_button");
    GtkWidget *delete_button = g_object_get_data(G_OBJECT(widget), "delete_message_button");
    if (event->crossing.detail == GDK_NOTIFY_INFERIOR) {
        return G_SOURCE_REMOVE;
    }
    
    if (event->type == GDK_ENTER_NOTIFY) {
        if (!gtk_widget_get_visible(edit_button)) {
            gtk_widget_show(edit_button);
        }
        if (!gtk_widget_get_visible(delete_button)) {
            gtk_widget_show(delete_button);
        }
    } 
    else if (event->type == GDK_LEAVE_NOTIFY) {
        if (gtk_widget_get_visible(edit_button)) {
            gtk_widget_hide(edit_button);
        }
        if (gtk_widget_get_visible(delete_button)) {
            gtk_widget_hide(delete_button);
        }
    }

    return G_SOURCE_REMOVE;
}

Message *message_encache(int id, int chat_id, int sender_id, char *text, time_t sent_time) {
    Message *res = malloc(sizeof(Message));

    printf("%s message added\n", text);
    res->message_id = id;
    res->sender_id = sender_id;
    res->text = strdup(text);
    res->sent_time = sent_time;
    res->message_box = NULL;
    res->chat_id = chat_id;
    res->file_id = -1;

    return res;
}

void message_free(Message *message) {
    if (!message) {
        return;
    }
    if (message->text) {
        free(message->text);
    }
    if (message->message_box) {
        gtk_widget_destroy(message->message_box);
    }
    
    message->message_id = -1;
    message->sender_id = -1;
    message->chat_id = -1;
}

void message_destroy(Message **message) {
    if (!message) {
        return;
    }
    g_hash_table_remove(main_data->messages_cache, GINT_TO_POINTER((*message)->message_id));
    message_free(*message);
    free(*message);
    *message = NULL;
}

static void on_delete_message_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;

    Message *message_data = user_data;

    Chat *current_chat = g_hash_table_lookup(main_data->chats_cache, GINT_TO_POINTER(message_data->chat_id));

    g_print("message id to delete: %d\n", message_data->message_id);
    g_print("chat id where is message to delete: %d\n", current_chat->chat_id);

    cJSON *json = form_edit_message_request(message_data->message_id, "Wow! The message is no more :(");
    send_request(json, USER_MESSAGE_EDIT); // When rt type will update change
}

void draw_message(gpointer key, gpointer value, gpointer user_data) {
    (void)key;
    (void)user_data;
    if (!value) {
        printf("Wow, a NULL message! GTK issue\n");
        return;
    }

    Message *message_data = (Message *)value;

    if (message_data == NULL) {
        printf("Wow, a NULL message! GTK issue\n");
        return;
    }
    
    Chat *message_chat = g_hash_table_lookup(main_data->chats_cache, GINT_TO_POINTER(message_data->chat_id));
    if (message_chat == NULL) {
        printf("Wow, a NULL chat! GTK issue\n");
        return;
    }
    
    User *message_sender = g_hash_table_lookup(main_data->users_cache, GINT_TO_POINTER(message_data->sender_id));
    if (message_sender == NULL) {
        printf("Wow, a NULL sender! GTK issue\n");
        return;
    }

    GtkWidget *message_container = message_chat->chat_message_container;

    message_data->message_box = gtk_event_box_new();
    GtkWidget *inner_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(message_data->message_box), inner_box);
    gtk_widget_set_events(message_data->message_box, GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK);
    
    const gchar *message_sender_name_text = message_sender->name;
    char time_string[20];
    timestamp_to_string(message_data->sent_time, time_string, sizeof(time_string));

    GtkWidget *message_avatar_image = gtk_image_new_from_pixbuf(message_sender->avatar);
    gtk_widget_set_halign(message_avatar_image, GTK_ALIGN_CENTER);

    g_object_set_data(G_OBJECT(message_data->message_box), "message_avatar_image", message_avatar_image);

    GtkWidget *message_text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(message_avatar_image, GTK_ALIGN_CENTER);

    g_object_set_data(G_OBJECT(message_data->message_box), "message_text_box", message_text_box);

    GtkWidget *message_name_edit_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(message_text_box), message_name_edit_box, FALSE, FALSE, 0);
    gtk_widget_set_name(message_name_edit_box, "message_name_edit_box");
    gtk_widget_set_halign(message_name_edit_box, GTK_ALIGN_FILL);

    GtkWidget *message_sender_name_label = gtk_label_new(message_sender_name_text);
    gtk_box_pack_start(GTK_BOX(message_name_edit_box), message_sender_name_label, TRUE, TRUE, 0);
    gtk_widget_set_name(message_sender_name_label, "message_sender_name_label");
    gtk_widget_set_halign(message_sender_name_label, GTK_ALIGN_START);

    g_object_set_data(G_OBJECT(message_data->message_box), "message_sender_name_label", message_sender_name_label);

    if (main_data->id == message_data->sender_id) {
        GtkWidget *message_edit_button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_box_pack_start(GTK_BOX(message_name_edit_box), message_edit_button_box, FALSE, FALSE, 0);
        gtk_widget_set_halign(message_edit_button_box, GTK_ALIGN_END);

        GtkWidget *edit_message_button = gtk_button_new(); 
        GtkWidget *edit_icon = gtk_image_new_from_file("img/edit_icon.svg");
        gtk_button_set_image(GTK_BUTTON(edit_message_button), edit_icon);
        gtk_button_set_always_show_image(GTK_BUTTON(edit_message_button), TRUE); 
        gtk_widget_set_name(edit_message_button, "button-transparent ");
        gtk_widget_set_margin_end(edit_message_button, 5);
        gtk_widget_set_margin_start(edit_message_button, 5);
        gtk_box_pack_start(GTK_BOX(message_name_edit_box), edit_message_button, FALSE, FALSE, 0);
        gtk_widget_set_no_show_all(edit_message_button, TRUE);

        GtkWidget *delete_message_button = gtk_button_new(); 
        GtkWidget *delete_icon = gtk_image_new_from_file("img/delete_icon.svg");
        gtk_button_set_image(GTK_BUTTON(delete_message_button), delete_icon); 
        gtk_button_set_always_show_image(GTK_BUTTON(delete_message_button), TRUE); 
        gtk_widget_set_name(delete_message_button, "button-transparent ");
        gtk_box_pack_start(GTK_BOX(message_name_edit_box), delete_message_button, FALSE, FALSE, 0);
        gtk_widget_set_no_show_all(delete_message_button, TRUE);

        g_object_set_data(G_OBJECT(message_data->message_box), "delete_message_button", delete_message_button);
        g_object_set_data(G_OBJECT(message_data->message_box), "edit_message_button", edit_message_button);

        g_signal_connect(delete_message_button, "clicked", G_CALLBACK(on_delete_message_button_clicked), message_data);
        g_signal_connect(edit_message_button, "clicked", G_CALLBACK(on_edit_message_button_clicked), message_data);
    }

    GtkWidget *message_message_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(message_text_box), message_message_box, FALSE, FALSE, 0);
    gtk_widget_set_halign(message_message_box, GTK_ALIGN_FILL);

    GtkWidget *message_message_label = gtk_label_new(message_data->text);
    g_object_set_data(G_OBJECT(message_data->message_box), "message_message_label", message_message_label);

    if (message_data->file_id == -1) {
        gtk_box_pack_start(GTK_BOX(message_message_box), message_message_label, TRUE, TRUE, 0);
        gtk_widget_set_halign(message_message_label, GTK_ALIGN_START);
        gtk_label_set_line_wrap(GTK_LABEL(message_message_label), TRUE);
        gtk_label_set_line_wrap_mode(GTK_LABEL(message_message_label), PANGO_WRAP_CHAR);
        gtk_label_set_max_width_chars(GTK_LABEL(message_message_label), 28);
        gtk_widget_set_name(message_message_label, "message_message_label");

    } 
    else {
        GtkWidget *file_message_button = gtk_button_new();
        gtk_widget_set_name(file_message_button, "file_message_button");
        gtk_widget_set_size_request(file_message_button, 48, 48);

        GtkWidget *file_message_image = gtk_image_new_from_file("img/file_message_button.svg");
        gtk_button_set_image(GTK_BUTTON(file_message_button), file_message_image);

        gtk_box_pack_start(GTK_BOX(message_message_box), file_message_button, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(message_message_box), message_message_label, FALSE, FALSE, 0);
        gtk_widget_set_name(message_message_label, "file_message_label");
    }

    GtkWidget *message_send_time_label = gtk_label_new(time_string);
    gtk_widget_set_name(message_send_time_label, "message_send_time_label");
    gtk_box_pack_start(GTK_BOX(message_message_box), message_send_time_label, FALSE, FALSE, 0);
    gtk_widget_set_valign(message_send_time_label, GTK_ALIGN_END);

    g_object_set_data(G_OBJECT(message_data->message_box), "message_send_time_label", message_send_time_label);

    gtk_list_box_insert(GTK_LIST_BOX(message_container), message_data->message_box, -1);

    if (main_data->id == message_data->sender_id) {
        gtk_box_pack_end(GTK_BOX(inner_box), message_avatar_image, FALSE, FALSE, 0);
        gtk_box_pack_end(GTK_BOX(inner_box), message_text_box, FALSE, FALSE, 0);
        gtk_widget_set_name(message_text_box, "my_message_text_box");
        gtk_widget_set_name(message_avatar_image, "my_message_avatar_image");

        g_signal_connect(message_data->message_box, "enter-notify-event", G_CALLBACK(on_hover_notify), NULL);
        g_signal_connect(message_data->message_box, "leave-notify-event", G_CALLBACK(on_hover_notify), NULL);
    } 
    else {
        gtk_box_pack_start(GTK_BOX(inner_box), message_avatar_image, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(inner_box), message_text_box, FALSE, FALSE, 0);
        gtk_widget_set_name(message_text_box, "message_text_box");
        gtk_widget_set_name(message_avatar_image, "message_avatar_image");
    }

    gtk_widget_show_all(message_data->message_box);
}

gboolean draw_single_message(gpointer user_data) {

    int message_id = GPOINTER_TO_INT(user_data);

    Message* message_data = g_hash_table_lookup(main_data->messages_cache, GINT_TO_POINTER(message_id));

    if (!message_data) {
        g_warning("Message with ID %d not found in cache.", message_id);
        return FALSE;
    }

    Chat *message_chat = g_hash_table_lookup(main_data->chats_cache, GINT_TO_POINTER(message_data->chat_id));

    if (message_chat == NULL) {
        printf("Wow, a NULL message! GTK issue\n");
        return FALSE;
    }

    draw_message(GINT_TO_POINTER(message_id), message_data, NULL);
    GtkWidget *outer_message_box = message_chat->chat_message_container;
    gtk_widget_show_all(outer_message_box);

    return G_SOURCE_REMOVE;
}

void destroy_single_message(int message_id) {
    Message *message_to_free = g_hash_table_lookup(main_data->messages_cache, GINT_TO_POINTER(message_id));

    if (message_to_free) {
        g_hash_table_remove(main_data->messages_cache, GINT_TO_POINTER(message_id));
        message_destroy(&message_to_free);
    }
}

void update_message_ui(gpointer key, gpointer value, gpointer user_data) {
    (void)key;
    (void)user_data;

    Message *message_data = (Message *)value;

    GtkWidget *message_message_label = g_object_get_data(G_OBJECT(message_data->message_box), "message_message_label");
    gtk_label_set_label(GTK_LABEL(message_message_label), message_data->text);

    //avatarko
}

void message_add_and_draw(int id, int chat_id, int sender_id, char *text, time_t send_time) {
    Message *message = message_encache(id, chat_id, sender_id, text, send_time);
    g_hash_table_insert(main_data->messages_cache, GINT_TO_POINTER(message->message_id), message);
    g_idle_add(draw_single_message, GINT_TO_POINTER(id));
}

void message_add(int id, int chat_id, int sender_id, char *text, time_t send_time) {
    Message *message = message_encache(id, chat_id, sender_id, text, send_time);
    g_hash_table_insert(main_data->messages_cache, GINT_TO_POINTER(message->message_id), message);
}

gboolean update_single_message(gpointer user_data) {
    Message *message_data = g_hash_table_lookup(main_data->messages_cache, user_data);
    if (!message_data) {
        g_warning("Message with ID %d not found in cache.", GPOINTER_TO_INT(user_data));
        return G_SOURCE_REMOVE;
    }
    update_message_ui(user_data, message_data, NULL);
    gtk_widget_show_all(message_data->message_box);
    return G_SOURCE_REMOVE;
}

