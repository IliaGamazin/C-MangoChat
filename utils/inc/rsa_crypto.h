#ifndef RSA_CRYPTO_H
#define RSA_CRYPTO_H

#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

// Structure for RSA keypair
typedef struct RSAKeyPair {
    EVP_PKEY *pkey;
} RSAKeyPair;

// Keypair generation
RSAKeyPair *generate_rsa_keypair(int bits);
void free_rsa_keypair(RSAKeyPair *keypair);

// Export and Load Keys
size_t export_public_key(EVP_PKEY *key, unsigned char **out);
unsigned char *rsa_public_key_to_pem(EVP_PKEY *public_key, size_t *pem_size);
EVP_PKEY *load_public_key_from_pem(const unsigned char *pem, size_t pem_size);

// Key Extraction and Encryption/Decryption
unsigned char *extract_public_key(EVP_PKEY *pkey, int *key_size);
bool load_public_key_from_payload(const unsigned char *payload, size_t payload_len, EVP_PKEY **public_key);
bool encrypt_with_rsa(EVP_PKEY *public_key, const unsigned char *data, size_t data_len, unsigned char **encrypted, size_t *encrypted_len);
bool decrypt_with_rsa(EVP_PKEY *private_key, const unsigned char *encrypted, size_t encrypted_len, unsigned char **decrypted, size_t *decrypted_len);

#endif // RSA_CRYPTO_H
