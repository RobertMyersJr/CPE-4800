/**********************************************
*  Filename: aes_ctr_gmac.c
*  Description: How to do openssl for DES
*  Author: Bob Turney
*  Date: 12/27/2024
*  Note: gcc -o aes_ctr_gmac aes_ctr_gmac.c -lssl -lcrypto
*
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}
int main(void) {
    // Plaintext
    unsigned char plaintext[] = "a message from Robert to alice"; // Key and IV
    unsigned char key[16] = {};
    unsigned char iv[16]; // CBC mode requires a 16-byte IV // Generate random key and IV
    if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
        handleErrors();
    } // Buffers for ciphertext and decrypted text
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    int decryptedtext_len, ciphertext_len;
    // GMAC tag
    unsigned char tag[16]; // Create and initialize the context for encryption
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    // Initialize encryption operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) handleErrors();
    // Provide the message to be encrypted, and obtain the encrypted output
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, plaintext, strlen((char *)plaintext))) handleErrors();
    // Finalize encryption
    int len;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + ciphertext_len, &len)) handleErrors();
    ciphertext_len += len;
    // Clean up encryption context
    EVP_CIPHER_CTX_free(ctx);
    // Create and initialize the context for GMAC
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    // Initialize GMAC operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, key, iv)) handleErrors();
    // Provide the ciphertext to GMAC
    if (1 != EVP_EncryptUpdate(ctx, NULL, &len, ciphertext, ciphertext_len)) handleErrors();
    // Finalize GMAC and get the tag
    if (1 != EVP_EncryptFinal_ex(ctx, NULL, &len)) handleErrors();
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag)) handleErrors();
    // Clean up GMAC context
    EVP_CIPHER_CTX_free(ctx);
    // Print plaintext, ciphertext, and GMAC tag
    printf("Plaintext: %s\n", plaintext);
    printf("Ciphertext: ");
    for (int i = 0; i < ciphertext_len; i++) printf("%02x", ciphertext[i]);
    printf("\nGMAC: ");
    for (int i = 0; i < 16; i++) printf("%02x", tag[i]);
    printf("\n");
    // Decryption
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    // Initialize decryption operation
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) handleErrors();
    // Provide the ciphertext to be decrypted, and obtain the plaintext output
    if (1 != EVP_DecryptUpdate(ctx, decryptedtext, &decryptedtext_len, ciphertext, ciphertext_len)) handleErrors();
    // Finalize decryption
    if (1 != EVP_DecryptFinal_ex(ctx, decryptedtext + decryptedtext_len, &len)) handleErrors();
    decryptedtext_len += len;
    // Add NULL terminator
    decryptedtext[decryptedtext_len] = '\0';
    // Clean up decryption context
    EVP_CIPHER_CTX_free(ctx);
    // Print decrypted text
    printf("Decrypted text: %s\n", decryptedtext);
    return 0;
}
