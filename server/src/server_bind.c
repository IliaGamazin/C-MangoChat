#include "../inc/server_ext.h"

int Server_bind(Server *server) {
    if (server == NULL) {
        return -1;
    }

    return bind(server->sd, server->ai->ai_addr, server->ai->ai_addrlen);
}

