#include "../inc/main_data.h"

MainData *MainData_new(GtkApplication *app, const char *node, const char *service) {
    MainData *res = malloc(sizeof(MainData));
    res->server = Server_new();
    if (!res->server) {
        fprintf(stderr, "Failed to create maindata!\n");
        free(res);
        return NULL;
    }

    if (Server_init(res->server, node, service) == -1) {
        fprintf(stderr, "Failed to create maindata!\n");
        Server_destroy(&(res->server));
        free(res);
        return NULL;
    }

    res->app = app;
    res->reconnect_delay = 5;
    res->is_closing = false;
    res->is_connected = false;
    res->buffer = gtk_text_buffer_new(NULL);
    res->thread_id = -1;

    res->id = -1;
    res->username = NULL;
    res->avatar = NULL;

    res->current_chat_id = -1;
    res->users_cache = g_hash_table_new(g_direct_hash, g_direct_equal);

    res->messages_cache = g_hash_table_new(g_direct_hash, g_direct_equal);
    res->message_order = NULL;

    res->chats_cache = g_hash_table_new(g_direct_hash, g_direct_equal);
    
    //User *ilka = user_encache(100, "Ilka", "default");
    // User *vlad = user_encache(101, "Vlad", "default");
    // User *rofa = user_encache(102, "Rofa", "default");

    // Chat *personal_ilka = chat_encache(0, 0, 2, ilka->name, "default");
    // Chat *personal_vlad = chat_encache(1, 2, 2, vlad->name, "default");
    // Chat *personal_rofa = chat_encache(2, 4, 2, rofa->name, "default");

    // Message *ilka_message_1 = message_encache(0, personal_ilka->chat_id, ilka->user_id, "Ilka said: balls", 1000195200);
    // Message *ilka_message_2 = message_encache(1, personal_ilka->chat_id, ilka->user_id, "Ilka said: balls once again", 1000195200);

    //Message *vlad_message_1 = message_encache(2, personal_vlad->chat_id, vlad->user_id, "Vlad said: balls", 1000195200);
   // Message *vlad_message_2 = message_encache(3, personal_vlad->chat_id, vlad->user_id, "Vlad said: balls once again", 1000195200);

    // Message *rofa_message_1 = message_encache(4, personal_rofa->chat_id, rofa->user_id, "Rofa said: balls", 1000195200);
    // Message *rofa_message_2 = message_encache(5, personal_rofa->chat_id, rofa->user_id, "Rofa said: balls once again", 1000195200);

   // Chat *group = chat_encache(3, -, 4, "Group", "default");

    // g_hash_table_insert(res->users_cache, GINT_TO_POINTER(ilka->user_id), ilka);
    // g_hash_table_insert(res->users_cache, GINT_TO_POINTER(vlad->user_id), vlad);
    // g_hash_table_insert(res->users_cache, GINT_TO_POINTER(rofa->user_id), rofa);

    // g_hash_table_insert(res->messages_cache, GINT_TO_POINTER(ilka_message_1->message_id), ilka_message_1);
    // g_hash_table_insert(res->messages_cache, GINT_TO_POINTER(ilka_message_2->message_id), ilka_message_2);

    // g_hash_table_insert(res->messages_cache, GINT_TO_POINTER(vlad_message_1->message_id), vlad_message_1);
    // g_hash_table_insert(res->messages_cache, GINT_TO_POINTER(vlad_message_2->message_id), vlad_message_2);

    // g_hash_table_insert(res->messages_cache, GINT_TO_POINTER(rofa_message_1->message_id), rofa_message_1);
    // g_hash_table_insert(res->messages_cache, GINT_TO_POINTER(rofa_message_2->message_id), rofa_message_2);

    // g_hash_table_insert(res->chats_cache, GINT_TO_POINTER(personal_ilka->chat_id), personal_ilka);
    // g_hash_table_insert(res->chats_cache, GINT_TO_POINTER(personal_vlad->chat_id), personal_vlad);
    // g_hash_table_insert(res->chats_cache, GINT_TO_POINTER(personal_rofa->chat_id), personal_rofa);

    pthread_mutex_init(&(res->mutex), NULL);
    
    return res;
}


void MainData_destroy(MainData *data) {
    if (!data) {
        return;
    }

    if (data->buffer) {
        g_object_unref(data->buffer);
    }
    
    if (data->avatar) {
        g_object_unref(data->avatar);
    }

    if (data->username) {
        free(data->username);
    }
    
    if (data->message_order) {
        g_list_free(data->message_order);
    }

    g_hash_table_destroy(data->users_cache);
    g_hash_table_destroy(data->messages_cache);
    g_hash_table_destroy(data->chats_cache);
    
    Server_destroy(&(data->server));
    pthread_mutex_destroy(&(data->mutex));

    free(data);
    printf("MainData destroyed!\n");
}

