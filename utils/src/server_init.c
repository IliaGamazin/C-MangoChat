#include "../inc/server.h"

int Server_init(Server *server, const char *node, const char *service) {
    if (server == NULL) {
        return -1;
    }

    if (Server_ai_init(server, node, service)) {
        Server_free(server);
        return -1;
    }

    if (Server_sd_init(server)) {
        Server_free(server);
        return -1;
    }

    return 0;
}

