#include "../inc/searchbar.h"

void lowkey_hide_chats(gpointer key, gpointer value, gpointer user_data) {
    (void)key;
    char *substr = (char*)user_data;
    Chat *chat_data = (Chat *)value;
    if (!chat_data->chat_box || !substr) {
        return;
    } 

    if (!strlen(substr)) {
        gtk_widget_show(chat_data->chat_box);
        return;
    }
    
    if (strstr(chat_data->name, substr)){
        gtk_widget_show(chat_data->chat_box);
        return;
    }

    gtk_widget_hide(chat_data->chat_box);
}

void lowkey_hide_messages(gpointer key, gpointer value, gpointer user_data) {
    (void)key;
    char *substr = (char*)user_data;
    Message *message_data = (Message *)value;
    if (!message_data->message_box || !substr) {
        return;
    } 

    if (!strlen(substr)) {
        gtk_widget_show(message_data->message_box);
        return;
    }
    
    if (strstr(message_data->text, substr)){
        gtk_widget_show(message_data->message_box);
        return;
    }

    gtk_widget_hide(message_data->message_box);
}

void on_search_chats_activate(GtkEntry *entry, gpointer user_data) {
    (void)user_data;
    const gchar *text = gtk_entry_get_text(entry);
    g_hash_table_foreach(main_data->chats_cache, lowkey_hide_chats, (gpointer)text);

    return;
}

void on_search_messages_activate(GtkEntry *entry, gpointer user_data) {
    (void)user_data;
    const gchar *text = gtk_entry_get_text(entry);
    g_hash_table_foreach(main_data->messages_cache, lowkey_hide_messages, (gpointer)text);
}

