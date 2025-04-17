#include "../inc/server_ext.h"

int Server_listen(Server *server, int limit) {
    if (server == NULL) {
        return -1;
    }

    return listen(server->sd, limit);
}

