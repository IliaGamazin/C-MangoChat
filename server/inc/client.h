#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <syslog.h>

// Client

typedef struct s_Client {
    int cd;
    int id;
    pthread_t tid;
    struct s_Client *next;
    // session data, blah blah blah
} Client;

Client *Client_new(void);
int Client_init(Client *client, int cd, pthread_t tid);
int Client_cd_init(Client *client, int cd);
int Client_tid_init(Client *client, pthread_t tid);
void Client_free(Client *client);
void Client_destroy(Client **client);
Client *Client_push_back(Client **clients, int cd);
Client *Client_find_by(const Client *clients, int (*cmp)(const Client *client, void *), void *arg);

// CMPs

int Client_by_tid(const Client *client, void *arg);
int Client_by_id(const Client *client, void *arg);

#endif

