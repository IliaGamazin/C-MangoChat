#include "../inc/handler.h"

Handler *Handler_new(DataBase *database, Client *clients, Client *client) {
    if (database == NULL || client == NULL) {
        return NULL;
    }

    Handler *handler = malloc(sizeof(Handler));
    if (handler == NULL) {
        return NULL;
    }

    handler->database = database;
    handler->clients = clients;
    handler->client = client;

    return handler;
}

