#include "../inc/payload.h"

Payload *Payload_new_from_file(const char *filepath) {
    Payload *payload = malloc(sizeof(Payload));
    if (payload == NULL) {
        return NULL;
    }

    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        Payload_destroy(&payload);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    payload->length = ftell(file);
    fseek(file, 0, SEEK_SET);

    payload->data = malloc(payload->length);
    if (payload->data == NULL) {
        fclose(file);
        Payload_destroy(&payload);
        return NULL;
    }

    fread(payload->data, payload->length, 1, file);
    fclose(file);

    return payload;
}

