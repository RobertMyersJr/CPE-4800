/**********************************************
*  Filename: aes_ctr.c
*  Description: How to do openssl for DES
*  Author: Bob Turney
*  Date: 12/27/2024
*  Note: gcc -o aes_ctr aes_ctr.c -lssl -lcrypto
*
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
void handleErrors(void) {
    fprintf(stderr, "An error occurred\n");
    exit(1);
}
int main() {
    // Key and IV
    unsigned char key[32]; // XTS mode requires a 256-bit key (two 128-bit keys)
    unsigned char iv[16];
    // Initialize key with incrementing values
    for (int i = 0; i < 32; i++) {
        key[i] = i;
    }
    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        handleErrors();
    } // Plaintext
    unsigned char plaintext[] = "Hello Robert!Hello Robert!Hello Robert!";
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    // Create and initialize the context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    // Initialize encryption operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_xts(), NULL, key, iv)) handleErrors();
    int len;
    int ciphertext_len;
    // Perform encryption
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, strlen((char *)plaintext))) handleErrors();
    ciphertext_len = len;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;
    // Print plaintext and ciphertext
    printf("Plaintext: %s\n", plaintext);
    printf("Ciphertext: ");
    for (int i = 0; i < ciphertext_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");
    EVP_CIPHER_CTX_free(ctx); // Reset context for decryption
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    // Initialize decryption operation
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_xts(), NULL, key, iv)) handleErrors();
    int decryptedtext_len;
    // Perform decryption
    if (1 != EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, ciphertext_len)) handleErrors();
    decryptedtext_len = len;
    if (1 != EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len)) handleErrors();
    decryptedtext_len += len;
    // Ensure decrypted text is null-terminated
    decryptedtext[decryptedtext_len] = '\0';
    // Print decrypted text
    printf("Decrypted text: %s\n", decryptedtext);
    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}
