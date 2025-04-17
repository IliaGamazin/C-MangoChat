#include "../inc/payload.h"

void Payload_free(Payload *payload) {
    if (payload == NULL) {
        return;
    }

    free(payload->data);
    payload->data = NULL;
}

