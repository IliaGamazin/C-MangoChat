#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

// Server

typedef struct s_Server {
    struct addrinfo *ai;
    int sd;
} Server;

Server *Server_new(void);
int Server_init(Server *server, const char *node, const char *service);
int Server_ai_init(Server *server, const char *node, const char *service);
int Server_sd_init(Server *server);
void Server_free(Server *server);
void Server_destroy(Server **server);

#endif
