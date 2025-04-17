#include "../inc/client.h"

int Client_tid_init(Client *client, pthread_t tid) {
    if (client == NULL) {
        return -1;
    }

    client->tid = tid;

    return 0;
}

