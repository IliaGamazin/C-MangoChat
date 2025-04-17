#include "../inc/messages.h"

void on_chat_entry_activate(GtkEntry *entry, gpointer user_data) {
    (void)user_data;

    if (main_data->current_chat_id == -1) {
        printf("Choose a chat before writing!\n");
        return;
    }
    
    time_t current_time = time(NULL);
    char time_string[20];
    
    timestamp_to_string(current_time, time_string, sizeof(time_string));
    const char *text = gtk_entry_get_text(entry);

    g_print("Entry activated with text: %s\n", text);
    g_print("Send time in timestamp: %s\n", time_string);

    cJSON *json = form_send_message_request((char*)text, false);
    send_request(json, USER_MESSAGE_CREATE);
    gtk_entry_set_text(entry, "");
}

void foreach_draw_message(void) {
    for (GList *l = main_data->message_order; l != NULL; l = l->next) {
        Message *m = g_hash_table_lookup(main_data->messages_cache, l->data);
        if (!m || !l->data) {
            printf("Null foreach message!\n");
            continue;
        }
        draw_message(l->data, m, NULL);
    }
}
