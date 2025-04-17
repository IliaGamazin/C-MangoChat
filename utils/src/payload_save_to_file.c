#include "../inc/payload.h"

int Payload_save_to_file(Payload *payload, const char *filepath) {
    if (payload == NULL) {
        return -1;
    }

    FILE *file = fopen(filepath, "wb");
    if (file == NULL) {
        return -1;
    }

    fwrite(payload->data, payload->length, 1, file);
    fclose(file);

    return 0;
}

