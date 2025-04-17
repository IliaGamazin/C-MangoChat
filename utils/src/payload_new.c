#include "../inc/payload.h"

Payload *Payload_new(size_t length, const char *data) {
    Payload *payload = malloc(sizeof(Payload));
    if (payload == NULL) {
        return NULL;
    }

    payload->length = length;
    payload->data = malloc(length);
    if (payload->data == NULL && length) {
        free(payload);
        return NULL;
    }

    if (data != NULL) {
        memcpy(payload->data, data, length);
    }

    return payload;
}

