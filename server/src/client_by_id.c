#include "../inc/client.h"

int Client_by_id(const Client *client, void *arg) {
    return client->id == *((int *) arg);
}

