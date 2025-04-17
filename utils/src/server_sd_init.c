#include "../inc/server.h"

int Server_sd_init(Server *server) {
    if (server == NULL) {
        return -1;
    }

    server->sd = socket(server->ai->ai_family, server->ai->ai_socktype, 0);
    if (server->sd == -1) {
        return -1;
    }

    return 0;
}

