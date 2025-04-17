#include "../inc/client.h"

void Client_free(Client *client) {
    if (client == NULL) {
        return;
    }

    close(client->cd);
    client->cd = -1;
}

