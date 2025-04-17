#include "../inc/handler.h"

void Handler_destroy(Handler **handler) {
    if (handler == NULL) {
        return;
    }

    Handler_free(*handler);
    free(*handler);
    *handler = NULL;
}

