#include "../inc/payload.h"

int Payload_base64_encode(Payload *payload) {
    if (payload == NULL) {
        return -1;
    }

    int status = -1;

    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *mem = BIO_new(BIO_s_mem());
    if (b64 == NULL || mem == NULL) {
        goto out_cleanup;
    }

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, mem);

    if (BIO_write(b64, payload->data, payload->length) <= 0 ||
        BIO_flush(b64) <= 0) {
        goto out_cleanup;
    }

    char *mem_data;
    long length = BIO_get_mem_data(b64, &mem_data);
    char *data = malloc(length + 1);
    if (data == NULL) {
        goto out_cleanup;
    }

    data[length] = '\0';

    Payload_free(payload);
    payload->data = memcpy(data, mem_data, length);
    payload->length = length;

    status = 0;

out_cleanup:
    BIO_free(mem);
    BIO_free(b64);

    return status;
}

