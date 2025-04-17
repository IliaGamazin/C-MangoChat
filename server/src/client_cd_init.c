#include "../inc/client.h"

int Client_cd_init(Client *client, int cd) {
    if (client == NULL) {
        return -1;
    }

    client->cd = cd;

    return 0;
}

