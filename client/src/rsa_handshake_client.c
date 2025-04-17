#include "../inc/rsa_handshake_client.h"

int rsa_handshake_client(int sock_fd) {
    RSAKeyPair *keypair = NULL; 
    unsigned char *public_key = NULL, *encrypted_key = NULL, *symmetric_key = NULL;
    int key_size;
    size_t symmetric_key_len = 0;
    int result = -1;

    // Generate temp RSA kpr wrapped in RSAKpr
    keypair = generate_rsa_keypair(2048);
    if (!keypair) {
        fprintf(stderr, "Failed to generate client RSA keypair\n");
        return -1;
    }

    // Extract public key in PEM format
    public_key = extract_public_key(keypair->pkey, &key_size); // Use keypair->pkey
    if (!public_key) {
        fprintf(stderr, "Failed to extract client public key\n");
        goto cleanup;
    }

    // Send public key to server
    if (send(sock_fd, public_key, key_size, 0) != key_size) {
        fprintf(stderr, "Failed to send client public key to server\n");
        goto cleanup;
    }

    // Receive encrypted symmetric key from server
    size_t encrypted_key_size = EVP_PKEY_get_size(keypair->pkey); // Use keypair->pkey
    encrypted_key = malloc(encrypted_key_size);
    if (!encrypted_key || recv(sock_fd, encrypted_key, encrypted_key_size, 0) <= 0) {
        fprintf(stderr, "Failed to receive encrypted symmetric key\n");
        goto cleanup;
    }

    // Decrypt symmetric key using client private RSA key
    if (!decrypt_with_rsa(keypair->pkey, encrypted_key, encrypted_key_size, &symmetric_key, &symmetric_key_len)) {
        fprintf(stderr, "Failed to decrypt symmetric key\n");
        goto cleanup;
    }

    printf("Successfully established a shared symmetric key\n");
    result = 0; // Success

cleanup:
    free_rsa_keypair(keypair); // Use free_rsa_keypair to clean up
    free(public_key);
    free(encrypted_key);
    free(symmetric_key);
    return result;
}
