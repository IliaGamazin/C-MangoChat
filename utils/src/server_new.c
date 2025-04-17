#include "../inc/server.h"

Server *Server_new(void) {
    Server *server = malloc(sizeof(Server));
    if (server == NULL) {
        return NULL;
    }

    server->ai = NULL;
    server->sd = -1;

    return server;
}

