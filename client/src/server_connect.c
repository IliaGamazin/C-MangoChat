#include "../inc/server_ext.h"
#include <errno.h>
#include <stdio.h>
#include <string.h> 

int Server_connect(Server *server) {
    if (server == NULL) {
        return -1;
    }

    if (connect(server->sd, server->ai->ai_addr, server->ai->ai_addrlen) == -1) {
        perror("Connection failed");
        return -1;
    }
    return 0;
}

