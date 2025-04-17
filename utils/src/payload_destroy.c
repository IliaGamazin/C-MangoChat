#include "../inc/payload.h"

void Payload_destroy(Payload **payload) {
    if (payload == NULL) {
        return;
    }

    Payload_free(*payload);
    free(*payload);
    *payload = NULL;
}

