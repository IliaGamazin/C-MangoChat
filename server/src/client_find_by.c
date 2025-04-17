#include "../inc/client.h"

Client *Client_find_by(const Client *clients, int (*cmp)(const Client *client, void *), void *arg) {
    while (clients != NULL) {
        if (cmp(clients, arg)) {
            return (Client *) clients;
        }
        clients = clients->next;
    }

    return NULL;
}

