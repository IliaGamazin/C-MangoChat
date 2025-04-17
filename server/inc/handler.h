#ifndef HANDLER_H
#define HANDLER_H

#include "./database.h"
#include "./client.h"
#include "./real_talk_ext.h"

// Handler

typedef struct s_Handler {
    DataBase *database;
    Client *clients;
    Client *client;
} Handler;

Handler *Handler_new(DataBase *database, Client *clients, Client *client);
void Handler_free(Handler *handler);
void Handler_destroy(Handler **handler);
void *Handler_routine(void *arg);

#endif

