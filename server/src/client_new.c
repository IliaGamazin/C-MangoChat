#include "../inc/client.h"

Client *Client_new(void) {
    Client *client = malloc(sizeof(Client));
    if (client == NULL) {
        return NULL;
    }

    client->cd = -1;
    client->id = -1;
    client->next = NULL;

    return client;
}

