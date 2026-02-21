/**********************************************
*  Filename: aes_ctr_gcm.c
*  Description: How to do openssl for DES
*  Author: Bob Turney
*  Date: 12/27/2024
*  Note: gcc -o aes_ctr_gcm aes_ctr_gcm.c -lssl -lcrypto
*
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
void print_hex(const char *label, const unsigned char *data, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}
int main() {
    const char *plaintext = "a message from Robert to alice";
    int plaintext_len = strlen(plaintext);
    // AES key (128 bits)
    unsigned char key[16] = "1234567891234567";
    // Initialization vector (IV)
    unsigned char iv[12] = "123456789123";
    // Buffers for ciphertext and decrypted text
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    int len, ciphertext_len, decryptedtext_len;
    // Buffer for GCM tag
    unsigned char tag[16];
    // Create and initialize the context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    // Initialize encryption operation
    EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(iv), NULL);
    EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
    // Encrypt the plaintext
    EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char *)plaintext, plaintext_len);
    ciphertext_len = len;
    // Finalize encryption
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;
    // Get the GCM tag
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, sizeof(tag), tag);
    // Print plaintext, ciphertext, and GCM tag
    printf("Plaintext: %s\n", plaintext);
    print_hex("Ciphertext", ciphertext, ciphertext_len);
    print_hex("GCM Tag", tag, sizeof(tag));
    // Initialize decryption operation
    EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(iv), NULL);
    EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);
    // Set expected GCM tag value
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, sizeof(tag), tag);
    // Decrypt the ciphertext
    EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, ciphertext_len);
    decryptedtext_len = len;
    // Finalize decryption
    if (EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len) > 0) {
        decryptedtext_len += len;
        decryptedtext[decryptedtext_len] = '\0'; // Null-terminate the decrypted text
        printf("Decrypted text: %s\n", decryptedtext);
    } else {
        printf("Decryption failed\n");
    }
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}
