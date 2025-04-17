#include "../inc/client.h"

int Client_init(Client *client, int cd, pthread_t tid) {
    if (client == NULL) {
        return -1;
    }

    if (Client_cd_init(client, cd)) {
        Client_free(client);
        return -1;
    }

    if (Client_tid_init(client, tid)) {
        Client_free(client);
        return -1;
    }

    return 0;
}


