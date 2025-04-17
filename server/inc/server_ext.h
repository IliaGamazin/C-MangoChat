#ifndef SERVER_EXT_H
#define SERVER_EXT_H

#include "../../utils/inc/server.h"

// ServerExt

int Server_bind(Server *server);
int Server_listen(Server *server, int limit);
int Server_accept(Server *server, struct sockaddr *restrict addr, socklen_t *restrict addrlen);

#endif
