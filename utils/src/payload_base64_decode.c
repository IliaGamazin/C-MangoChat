#include "../inc/payload.h"

int Payload_base64_decode(Payload *payload) {
    if (payload == NULL) {
        return -1;
    }

    int status = -1;

    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *mem = BIO_new_mem_buf(payload->data, payload->length);
    if (b64 == NULL || mem == NULL) {
        goto out_cleanup;
    }

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, mem);

    char *data = malloc(payload->length);
    if (data == NULL) {
        goto out_cleanup;
    }

    long length = BIO_read(b64, data, payload->length);
    if (length <= 0) {
        free(data);
        goto out_cleanup;
    }

    Payload_free(payload);
    payload->data = realloc(data, length);
    payload->length = length;

    status = 0;

out_cleanup:
    BIO_free(mem);
    BIO_free(b64);

    return status;
}

