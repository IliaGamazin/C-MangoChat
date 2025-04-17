#include "../inc/payload.h"

Payload *Payload_new_own(size_t length, char *data) {
    Payload *payload = malloc(sizeof(Payload));
    if (payload == NULL) {
        return NULL;
    }

    payload->length = length;
    payload->data = data;

    return payload;
}

