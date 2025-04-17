#include "../inc/rsa_handshake_server.h"

void handle_rsa_handshake(int client_fd) {
    RSAKeyPair *keypair = NULL;
    unsigned char *pubkey_pem = NULL, *symmetric_key = NULL;
    size_t pubkey_len = 0, symmetric_key_len = 0;
    unsigned char encrypted_key[256];
    ssize_t received;

    // Generate RSA keypair
    keypair = generate_rsa_keypair(2048);
    if (!keypair) {
        fprintf(stderr, "Failed to generate RSA keypair\n");
        return;
    }

    // Convert public key to PEM format
    pubkey_pem = rsa_public_key_to_pem(keypair->pkey, &pubkey_len);
    if (!pubkey_pem) {
        free_rsa_keypair(keypair);
        fprintf(stderr, "Failed to convert public key to PEM\n");
        return;
    }

    // Send public key to the client
    if (send(client_fd, pubkey_pem, pubkey_len, 0) != (ssize_t)pubkey_len) {
        free(pubkey_pem);
        free_rsa_keypair(keypair);
        fprintf(stderr, "Failed to send public key to client\n");
        return;
    }
    free(pubkey_pem);

    // Receive the encrypted symmetric key from the client
    received = recv(client_fd, encrypted_key, sizeof(encrypted_key), 0);
    if (received <= 0) {
        free_rsa_keypair(keypair);
        fprintf(stderr, "Failed to receive encrypted symmetric key from client\n");
        return;
    }

    // Decrypt the symmetric key
    if (!decrypt_with_rsa(keypair->pkey, encrypted_key, (size_t)received, &symmetric_key, &symmetric_key_len)) {
        free_rsa_keypair(keypair);
        fprintf(stderr, "Failed to decrypt the symmetric key\n");
        return;
    }

    // Use symmetric_key and symmetric_key_len for further communication...
    printf("Symmetric key successfully decrypted.\n");

    // Clean up
    free(symmetric_key);
    free_rsa_keypair(keypair);
}

