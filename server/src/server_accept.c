#include "../inc/server_ext.h"

int Server_accept(Server *server, struct sockaddr *restrict addr, socklen_t *restrict addrlen) {
    if (server == NULL) {
        return -1;
    }

    return accept(server->sd, addr, addrlen);
}

