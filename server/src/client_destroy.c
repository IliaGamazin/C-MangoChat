#include "../inc/client.h"

void Client_destroy(Client **client) {
    if (client == NULL || *client == NULL) {
        return;
    }

    // Doesn't exist now
    (*client)->id = -1;
}

