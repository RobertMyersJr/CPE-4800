/**********************************************
*  Filename: aes_cbc_padded.c
*  Description: How to do openssl for DES
*  Author: Bob Turney
*  Date: 12/27/2024
*  Note: gcc -o aes_cbc_padded aes_cbc_padded.c -lssl -lcrypto
*
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}
int main(void) {
    // Plaintext
    unsigned char plaintext[] = "Hello Robert!Hello Robert!";
    int plaintext_len = strlen((char *)plaintext);
    // Key and IV
    unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    unsigned char iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    // Buffers for ciphertext and decrypted text
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    int decryptedtext_len, ciphertext_len;
    // Initialize the library
    if (OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL) != 1) {
        handleErrors();
    }
    // Encrypt the plaintext
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, plaintext, plaintext_len))
        handleErrors();
    int len;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + ciphertext_len, &len))
        handleErrors();
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    // Decrypt the ciphertext
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();
    if (1 != EVP_DecryptUpdate(ctx, decryptedtext, &decryptedtext_len, ciphertext, ciphertext_len))
        handleErrors();
    if (1 != EVP_DecryptFinal_ex(ctx, decryptedtext + decryptedtext_len, &len))
        handleErrors();
    decryptedtext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    // Null-terminate the decrypted text
    decryptedtext[decryptedtext_len] = '\0';
    // Show the results
    printf("Plaintext: %s\n", plaintext);
    printf("Ciphertext: ");
    for (int i = 0; i < ciphertext_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");
    printf("Decrypted text: %s\n", decryptedtext);
    // Clean up
    EVP_cleanup();
    return 0;
}
