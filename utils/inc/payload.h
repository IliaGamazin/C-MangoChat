#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>

// Payload

typedef struct s_Payload {
    size_t length;
    char *data;
} Payload;

Payload *Payload_new(size_t length, const char *data);
Payload *Payload_new_own(size_t length, char *data);
Payload *Payload_new_from_file(const char *filepath);
void Payload_free(Payload *payload);
void Payload_destroy(Payload **payload);
int Payload_save_to_file(Payload *payload, const char *filepath);
int Payload_base64_encode(Payload *payload);
int Payload_base64_decode(Payload *payload);

#endif

