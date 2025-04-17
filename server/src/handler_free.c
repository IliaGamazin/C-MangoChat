#include "../inc/handler.h"

void Handler_free(Handler *handler) {
    if (handler == NULL) {
        return;
    }

    DataBase_destroy(&handler->database);
    Client_destroy(&handler->client);
}

