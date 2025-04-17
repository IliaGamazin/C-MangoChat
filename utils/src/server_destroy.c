#include "../inc/server.h"

void Server_destroy(Server **server) {
    if (server == NULL) {
        return;
    }

    Server_free(*server);
    free(*server);
    *server = NULL;
}

