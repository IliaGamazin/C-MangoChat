#include "../inc/server.h"

int Server_ai_init(Server *server, const char *node, const char *service) {
    if (server == NULL) {
        return -1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int gai_errno = getaddrinfo(node, service, &hints, &server->ai);
    if (gai_errno) {
        return -1;
    }

    return 0;
}

