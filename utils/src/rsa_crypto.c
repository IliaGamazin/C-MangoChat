#include "../inc/rsa_crypto.h"

RSAKeyPair *generate_rsa_keypair(int bits) {
    if (bits <= 0) return NULL;

    RSAKeyPair *keypair = malloc(sizeof(RSAKeyPair));
    if (!keypair) return NULL;

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0 || EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        free(keypair);
        return NULL;
    }

    if (EVP_PKEY_keygen(ctx, &keypair->pkey) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        free(keypair);
        return NULL;
    }

    EVP_PKEY_CTX_free(ctx);
    return keypair;
}

void free_rsa_keypair(RSAKeyPair *keypair) {
    if (!keypair) return;
    if (keypair->pkey) EVP_PKEY_free(keypair->pkey);
    free(keypair);
}

unsigned char *extract_public_key(EVP_PKEY *pkey, int *key_size) {
    if (!pkey || !key_size) return NULL;

    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) return NULL;

    if (!PEM_write_bio_PUBKEY(bio, pkey)) {
        BIO_free(bio);
        return NULL;
    }

    BUF_MEM *buf;
    BIO_get_mem_ptr(bio, &buf);

    unsigned char *public_key = malloc(buf->length);
    if (!public_key) {
        BIO_free(bio);
        return NULL;
    }

    memcpy(public_key, buf->data, buf->length);
    *key_size = (int)buf->length;

    BIO_free(bio);
    return public_key;
}

unsigned char *rsa_public_key_to_pem(EVP_PKEY *public_key, size_t *pem_size) {
    if (!public_key || !pem_size) return NULL;

    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) return NULL;

    if (!PEM_write_bio_PUBKEY(bio, public_key)) {
        BIO_free(bio);
        return NULL;
    }

    BUF_MEM *mem;
    BIO_get_mem_ptr(bio, &mem);

    unsigned char *pem = malloc(mem->length + 1);
    if (!pem) {
        BIO_free(bio);
        return NULL;
    }

    memcpy(pem, mem->data, mem->length);
    pem[mem->length] = '\0';
    *pem_size = mem->length;

    BIO_free(bio);
    return pem;
}

EVP_PKEY *load_public_key_from_pem(const unsigned char *pem, size_t pem_size) {
    BIO *bio = BIO_new_mem_buf(pem, pem_size);
    if (!bio) return NULL;

    EVP_PKEY *public_key = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);
    return public_key;
}

bool encrypt_with_rsa(EVP_PKEY *public_key, const unsigned char *data, size_t data_len, unsigned char **encrypted, size_t *encrypted_len) {
    if (!public_key || !data || !encrypted || !encrypted_len) return false;

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(public_key, NULL);
    if (!ctx || EVP_PKEY_encrypt_init(ctx) <= 0 || EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    if (EVP_PKEY_encrypt(ctx, NULL, encrypted_len, data, data_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    *encrypted = malloc(*encrypted_len);
    if (!*encrypted || EVP_PKEY_encrypt(ctx, *encrypted, encrypted_len, data, data_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        free(*encrypted);
        return false;
    }

    EVP_PKEY_CTX_free(ctx);
    return true;
}

bool decrypt_with_rsa(EVP_PKEY *private_key, const unsigned char *encrypted, size_t encrypted_len, unsigned char **decrypted, size_t *decrypted_len) {
    if (!private_key || !encrypted || !decrypted || !decrypted_len) return false;

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(private_key, NULL);
    if (!ctx || EVP_PKEY_decrypt_init(ctx) <= 0 || EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    if (EVP_PKEY_decrypt(ctx, NULL, decrypted_len, encrypted, encrypted_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    *decrypted = malloc(*decrypted_len);
    if (!*decrypted || EVP_PKEY_decrypt(ctx, *decrypted, decrypted_len, encrypted, encrypted_len) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        free(*decrypted);
        return false;
    }

    EVP_PKEY_CTX_free(ctx);
    return true;
}
