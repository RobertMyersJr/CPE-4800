/**********************************************
*  Filename: aes_cbc.c
*  Description: How to do openssl for DES
*  Author: Bob Turney
*  Date: 12/27/2024
*  Note: gcc -o aes_cbc aes_cbc.c -lssl -lcrypto
*
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

void handleErrors(void) {
    fprintf(stderr, "An error occurred\n");
    exit(1);
}

int main() {
    // Incrementing key and zero IV
    unsigned char key[AES_BLOCK_SIZE];
    unsigned char iv[AES_BLOCK_SIZE] = {0};

    // Initialize key with incrementing values
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        key[i] = i;
    }

    // Plaintext
    unsigned char plaintext[] = "Hello Robert!";
    unsigned char ciphertext[AES_BLOCK_SIZE];
    unsigned char decryptedtext[AES_BLOCK_SIZE];

    // AES-128 encryption
    AES_KEY enc_key;
    AES_set_encrypt_key(key, 128, &enc_key);
    AES_cbc_encrypt(plaintext, ciphertext, sizeof(plaintext), &enc_key, iv, AES_ENCRYPT);

    // Print plaintext and ciphertext
    printf("Plaintext: %s\n", plaintext);
    printf("Ciphertext: ");
    for (int i = 0; i < sizeof(ciphertext); i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // Reset IV for decryption
    memset(iv, 0, AES_BLOCK_SIZE);

    // AES-128 decryption
    AES_KEY dec_key;
    AES_set_decrypt_key(key, 128, &dec_key);
    AES_cbc_encrypt(ciphertext, decryptedtext, sizeof(ciphertext), &dec_key, iv, AES_DECRYPT);

    // Ensure decrypted text is null-terminated
    decryptedtext[sizeof(plaintext) - 1] = '\0';

    // Print decrypted text
    printf("Decrypted text: %s\n", decryptedtext);

    return 0;
}
