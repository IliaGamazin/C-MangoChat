#ifndef MAIN_DATA_H
#define MAIN_DATA_H

#include "../inc/libs.h"
#include "../inc/server_ext.h"
#include "../inc/session_cache.h"

typedef struct {
    GtkApplication *app;
    Server *server;
    int reconnect_delay;
    bool is_closing;
    bool is_connected;
    GtkTextBuffer *buffer;
    pthread_mutex_t mutex;
    pthread_t thread_id;

    int id;
    char *username;
    GdkPixbuf *avatar;
    
    int current_chat_id;
    GHashTable *chats_cache;
    GHashTable *users_cache;

    GHashTable *messages_cache;
    GList *message_order;
} MainData;

extern MainData *main_data;

MainData *MainData_new(GtkApplication *app, const char *node, const char *service);
void MainData_destroy(MainData *data);

#endif
