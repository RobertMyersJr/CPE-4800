/**********************************************
*  Filename: aes_cbc_mac.c
*  Description: How to do openssl for DES
*  Author: Bob Turney
*  Date: 12/27/2024
*  Note: gcc -o aes_cbc_mac aes_cbc_mac.c -lssl -lcrypto
*
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/cmac.h>
// Function to print bytes in hexadecimal
void print_hex(const char *label, const unsigned char *data, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}
// Function to calculate CBC-MAC
void calculate_cbc_mac(const unsigned char *key, const unsigned char *data, int data_len, unsigned char *mac) {
    AES_KEY enc_key;
    AES_set_encrypt_key(key, 128, &enc_key);

    unsigned char iv[AES_BLOCK_SIZE] = {0}; // IV is zero for CBC-MAC
    unsigned char buffer[AES_BLOCK_SIZE] = {0};

    for (int i = 0; i < data_len; i += AES_BLOCK_SIZE) {
        for (int j = 0; j < AES_BLOCK_SIZE; j++) {
            buffer[j] ^= data[i + j];
        }
        AES_encrypt(buffer, buffer, &enc_key);
    }
    memcpy(mac, buffer, AES_BLOCK_SIZE);
}
int main() {
    // Plaintext
    const char *plaintext = "a message from robert to preston";
    int plaintext_len = strlen(plaintext);
    // AES key (128 bits)
    unsigned char key[AES_BLOCK_SIZE] = "0123456789abcdef";
    // Initialization vector (IV)
    unsigned char iv[AES_BLOCK_SIZE] = "abcdef9876543210";
    unsigned char iv_dec[AES_BLOCK_SIZE];
    memcpy(iv_dec, iv, AES_BLOCK_SIZE); // Copy IV for decryption
    // Buffers for ciphertext and decrypted text
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    // AES encryption
    AES_KEY enc_key, dec_key;
    AES_set_encrypt_key(key, 128, &enc_key);
    AES_cbc_encrypt((unsigned char *)plaintext, ciphertext, plaintext_len, &enc_key, iv, AES_ENCRYPT);
    // Print plaintext and ciphertext
    printf("Plaintext: %s\n", plaintext);
    print_hex("Ciphertext", ciphertext, plaintext_len);
    // CBC-MAC calculation
    unsigned char cbc_mac_value[AES_BLOCK_SIZE];
    calculate_cbc_mac(key, ciphertext, plaintext_len, cbc_mac_value);
    // Print CBC-MAC value
    print_hex("CBC-MAC Value", cbc_mac_value, AES_BLOCK_SIZE);
    // AES decryption
    AES_set_decrypt_key(key, 128, &dec_key);
    AES_cbc_encrypt(ciphertext, decryptedtext, plaintext_len, &dec_key, iv_dec, AES_DECRYPT);
    decryptedtext[plaintext_len] = '\0'; // Null-terminate the decrypted text
    // Print decrypted text
    printf("Decrypted text: %s\n", decryptedtext);
    return 0;
}
