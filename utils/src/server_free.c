#include "../inc/server.h"

void Server_free(Server *server) {
    if (server == NULL) {
        return;
    }

    freeaddrinfo(server->ai);
    server->ai = NULL;
    close(server->sd);
    server->sd = -1;
}

