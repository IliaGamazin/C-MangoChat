#include "../inc/client.h"

Client *Client_push_back(Client **list, int cd) {
    if (list == NULL) {
        return NULL;
    }

    Client *node = Client_new();
    if (node == NULL) {
        return NULL;
    }

    Client_cd_init(node, cd);

    if (*list == NULL) {
        *list = node;
        return node;
    }

    Client *last = *list;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = node;

    return node;
}

