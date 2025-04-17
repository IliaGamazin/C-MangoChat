#include "../inc/session_cache.h"

Chat *chat_encache(int id, int last_message_id, int members_num, char *name, const char *encoded_avatar) {
    Chat *res = malloc(sizeof(Chat));
    res->chat_id = id;
    res->last_message_id = last_message_id;
    res->members_num = members_num;
    res->name = strdup(name);
    res->chat_box = NULL;
    res->chat_message_container = NULL;

    if (encoded_avatar) {
        unsigned long stream_size;
        unsigned char *decoded_stream = g_base64_decode(encoded_avatar, &stream_size);
        res->avatar = create_rounded_pixbuf_from_memory (decoded_stream, stream_size, 42, 42);
        g_free(decoded_stream);
    }
    else {
        res->avatar = create_rounded_pixbuf("img/placeholder_avatar.svg", 42, 42);
    }

    return res;
}

void chat_free(Chat *chat) {
    if (!chat) {
        return;
    }
    if (chat->name) {
        free(chat->name);
    }
    if (chat->chat_box && GTK_IS_WIDGET(chat->chat_box)) {
        gtk_widget_destroy(chat->chat_box);
    }
    chat = NULL;
}

void chat_destroy(Chat **chat) {
    if (!chat) {
        return;
    }
    g_hash_table_remove(main_data->chats_cache, GINT_TO_POINTER((*chat)->chat_id));
    chat_free(*chat);
    free(*chat);
    *chat = NULL;
}

void on_chat_click(GtkWidget *button, gpointer data) {
    (void)button;
    int new_id = GPOINTER_TO_INT(data);
    int old_id = main_data->current_chat_id;

    if (old_id == new_id) {
        return;
    }

    Chat *next_chat = g_hash_table_lookup(main_data->chats_cache, GINT_TO_POINTER(new_id));
    GtkWidget *next_message_container_box = next_chat->chat_message_container;
    GtkWidget *next_chat_button = g_object_get_data(G_OBJECT(next_chat->chat_box), "chat_button");
    
    if (old_id != -1) {
        Chat *current_chat = g_hash_table_lookup(main_data->chats_cache, GINT_TO_POINTER(old_id));
        GtkWidget *current_message_container_box = current_chat->chat_message_container;
        GtkWidget *current_chat_button = g_object_get_data(G_OBJECT(current_chat->chat_box), "chat_button");
        gtk_widget_set_name(current_chat_button, "chat_button");

        gtk_widget_hide(current_message_container_box);
    }

    gtk_widget_show(next_message_container_box);
    gtk_widget_set_name(next_chat_button, "chat_button_selected");

    GtkWidget *topbar = g_object_get_data(G_OBJECT(main_data->buffer), "topbar_box");
    GtkWidget *topbar_name_label = g_object_get_data(G_OBJECT(topbar), "chat_name_topbar");
    GtkWidget *avatar_image = g_object_get_data(G_OBJECT(topbar), "avatar_icon_topbar");
    
    gtk_image_set_from_pixbuf(GTK_IMAGE(avatar_image), next_chat->avatar);

    gtk_label_set_text(GTK_LABEL(topbar_name_label), next_chat->name);

    main_data->current_chat_id = new_id;

    g_print("Contact clicked ID: %d\n", new_id);
}

void draw_chat(gpointer key, gpointer value, gpointer user_data) {
    if (!value) {
        printf("Wow, a NULL chat! GTK issue\n");
        return;
    }
    
    (void)user_data;
    GtkWidget *time_label = NULL;
    GtkWidget *last_message_label = NULL;

    Chat *chat_data = (Chat *)value; 
    GHashTable *messages_table = main_data->messages_cache;
    GtkWidget *outer_chat_box = g_object_get_data(G_OBJECT(main_data->buffer), "chat_list_scroll");
    GtkWidget *inner_chat_box = g_object_get_data(G_OBJECT(outer_chat_box), "chat_list_box");
    GtkWidget *chat_message_container_box = g_object_get_data(G_OBJECT(main_data->buffer), "chat_message_container_box");

    chat_data->chat_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *chat_button = gtk_button_new();
    gtk_box_pack_start(GTK_BOX(chat_data->chat_box), chat_button, TRUE, TRUE, 0);
    gtk_widget_set_name(chat_button, "chat_button");

    GtkWidget *chat_box_contains = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(chat_box_contains, 296, 65); 

    //avatar not available at the moment
    GtkWidget *chat_avatar_image = gtk_image_new_from_pixbuf(chat_data->avatar);
    gtk_box_pack_start(GTK_BOX(chat_box_contains), chat_avatar_image, FALSE, FALSE, 0);
    gtk_widget_set_name(chat_avatar_image, "chat_avatar_image");
    gtk_widget_set_halign(chat_avatar_image, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(chat_avatar_image, GTK_ALIGN_CENTER);

    GtkWidget *chat_text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(chat_box_contains), chat_text_box, TRUE, TRUE, 0);
    gtk_widget_set_halign(chat_text_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(chat_text_box, GTK_ALIGN_CENTER);

    GtkWidget *name_time_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(chat_text_box), name_time_label_box, FALSE, FALSE, 0);
    gtk_widget_set_halign(name_time_label_box, GTK_ALIGN_FILL);

    GtkWidget *name_label = gtk_label_new(chat_data->name);
    gtk_box_pack_start(GTK_BOX(name_time_label_box), name_label, TRUE, TRUE, 0);
    gtk_widget_set_name(name_label, "name_label");
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_label_set_max_width_chars(GTK_LABEL(name_label),(24));
    gtk_label_set_ellipsize(GTK_LABEL(name_label), PANGO_ELLIPSIZE_END);

    time_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(name_time_label_box), time_label, FALSE, FALSE, 0);
    gtk_widget_set_name(time_label, "time_label");
    gtk_widget_set_halign(time_label, GTK_ALIGN_END);

    last_message_label = gtk_label_new("No messages yet");
    gtk_box_pack_start(GTK_BOX(chat_text_box), last_message_label, FALSE, FALSE, 0);
    gtk_widget_set_name(last_message_label, "last_message_label");
    gtk_widget_set_halign(last_message_label, GTK_ALIGN_START);
    gtk_label_set_max_width_chars(GTK_LABEL(last_message_label), 32);
    gtk_label_set_ellipsize(GTK_LABEL(last_message_label), PANGO_ELLIPSIZE_END);

    Message *last_message = g_hash_table_lookup(messages_table, GINT_TO_POINTER(chat_data->last_message_id));
    chat_data->chat_message_container = gtk_list_box_new();
    gtk_box_pack_start(GTK_BOX(chat_message_container_box), chat_data->chat_message_container, FALSE, FALSE, 0);

    if (last_message) {
        char time_string[20];
        timestamp_to_string(last_message->sent_time, time_string, sizeof(time_string));
        gtk_label_set_label(GTK_LABEL(last_message_label), last_message->text);
        gtk_label_set_label(GTK_LABEL(time_label), time_string);
    }

    g_object_set_data(G_OBJECT(main_data->buffer), "chat_message_container", chat_data->chat_message_container);
    g_object_set_data(G_OBJECT(chat_data->chat_box), "chat_button", chat_button);
    g_object_set_data(G_OBJECT(chat_data->chat_box), "name_label", name_label);
    g_object_set_data(G_OBJECT(chat_data->chat_box), "last_message_label", last_message_label);
    g_object_set_data(G_OBJECT(chat_data->chat_box), "time_label", time_label);

    gtk_container_add(GTK_CONTAINER(chat_button), chat_box_contains);
    gtk_list_box_insert(GTK_LIST_BOX(inner_chat_box), chat_data->chat_box, -1);

    gtk_widget_set_no_show_all(chat_data->chat_message_container, TRUE);

    gtk_widget_show_all(chat_button);

    g_signal_connect(chat_button, "clicked", G_CALLBACK(on_chat_click), key);
}

gboolean draw_single_chat(gpointer user_data) {
    Chat *chat_data = g_hash_table_lookup(main_data->chats_cache, user_data);
    if (!chat_data) {
        g_warning("Chat with ID %d not found in cache.", GPOINTER_TO_INT(user_data));
        return G_SOURCE_REMOVE;
    }

    draw_chat(user_data, chat_data, NULL);
    GtkWidget *outer_chat_box = g_object_get_data(G_OBJECT(main_data->buffer), "chat_list_scroll");
    gtk_widget_show_all(outer_chat_box);
    return G_SOURCE_REMOVE;
}

void destroy_single_chat(int chat_id) {
    Chat *chat_to_free = g_hash_table_lookup(main_data->chats_cache, GINT_TO_POINTER(chat_id));
    if (chat_to_free) {
        g_hash_table_remove(main_data->chats_cache, GINT_TO_POINTER(chat_id));
        chat_destroy(&chat_to_free);
    }
}

void update_chat_ui(gpointer key, gpointer value, gpointer user_data) {
    (void)user_data;
    (void)key;

    Chat *chat_data = (Chat *)value; 
    GHashTable *messages_table = main_data->messages_cache;
    GtkWidget *name_label = g_object_get_data(G_OBJECT(chat_data->chat_box), "name_label");
    gtk_label_set_label(GTK_LABEL(name_label), chat_data->name);

    Message *last_message = g_hash_table_lookup(messages_table, GINT_TO_POINTER(chat_data->last_message_id));
    const char *last_message_text = last_message ? last_message->text : "No messages";

    GtkWidget *last_message_label = g_object_get_data(G_OBJECT(chat_data->chat_box), "last_message_label");
    gtk_label_set_label(GTK_LABEL(last_message_label), last_message_text);

    GtkWidget *time_label = g_object_get_data(G_OBJECT(chat_data->chat_box), "time_label");

    char time_string[20];
    timestamp_to_string(last_message->sent_time, time_string, sizeof(time_string));
    gtk_label_set_label(GTK_LABEL(time_label), time_string);
}

gboolean update_single_chat(gpointer user_data) {

    Chat *chat_data = g_hash_table_lookup(main_data->chats_cache, user_data);

    if (!chat_data) {
        g_warning("Chat with ID %d not found in cache.", GPOINTER_TO_INT(user_data));
        return G_SOURCE_REMOVE;
    }

    update_chat_ui(NULL, chat_data, NULL);
    GtkWidget *outer_chat_box = g_object_get_data(G_OBJECT(main_data->buffer), "chat_list_scroll");
    gtk_widget_show_all(outer_chat_box);
    return G_SOURCE_REMOVE;
}

void chat_add_and_draw(int id, int last_message_id, int members_num, char *name, const char *encoded_avatar) {
    Chat *chat = chat_encache(id, last_message_id, members_num, name, encoded_avatar);
    g_hash_table_insert(main_data->chats_cache, GINT_TO_POINTER(chat->chat_id), chat);
    g_idle_add(draw_single_chat, GINT_TO_POINTER(id));
}

void chat_add(int id, int last_message_id, int members_num, char *name, const char *encoded_avatar) {
    Chat *chat = chat_encache(id, last_message_id, members_num, name, encoded_avatar);
    g_hash_table_insert(main_data->chats_cache, GINT_TO_POINTER(chat->chat_id), chat);
}
