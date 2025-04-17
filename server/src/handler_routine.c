#include "../inc/handler.h"

void *Handler_routine(void *arg) {
    Handler *handler = arg;
    DataBase *database = handler->database;
    Client *clients = handler->clients;
    Client *client = handler->client;

    syslog(LOG_INFO, "Client %d connected", client->cd);
    RealTalk *request = NULL;
    while ((request = RealTalk_receive(client->cd)) != NULL) {
        RealTalk_respond(request, clients, client, database);
        RealTalk_destroy(&request);
    }

    syslog(LOG_INFO, "Client %d disconnected", client->cd);
    Handler_destroy(&handler);
    pthread_exit(NULL);
}

