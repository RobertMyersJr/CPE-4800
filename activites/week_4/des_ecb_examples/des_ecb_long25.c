/**********************************************
*  Filename: des_ecb_long25.c
*  Description: How to do DES for book example
*  Author: Bob Turney
*  Date: 12/27/2024
*  Note: gcc -o des_ecb_long25 des_ecb_long25.c -lssl -lcrypto
*
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/des.h>

void print_data(const char *title, const void* data, int len) {
    printf("%s: ", title);
    const unsigned char *p = (const unsigned char*)data;
    for (int i = 0; i < len; i++) {
        printf("%02X ", *p++);
    }
    printf("\n");
}

int main() {
    // Key
    DES_cblock key = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF};
    DES_key_schedule schedule;

    // Set the key
    DES_set_key_unchecked(&key, &schedule);

    // Plaintext
    const char* plaintext = "HiRobertHiRobertGood";
    unsigned char ciphertext[64];
    unsigned char decryptedtext[64];

    // Encrypt
    for (int i = 0; i < strlen(plaintext); i += 8) {
        DES_ecb_encrypt((DES_cblock*)(plaintext + i), (DES_cblock*)(ciphertext + i), &schedule, DES_ENCRYPT);
    }

    // Decrypt
    for (int i = 0; i < strlen(plaintext); i += 8) {
        DES_ecb_encrypt((DES_cblock*)(ciphertext + i), (DES_cblock*)(decryptedtext + i), &schedule, DES_DECRYPT);
    }

    // Add null terminator to decrypted text
    decryptedtext[strlen(plaintext)] = '\0';

    // Print results
    printf("Plaintext: %s\n", plaintext);
    print_data("Ciphertext", ciphertext, strlen(plaintext));
    printf("Decrypted: %s\n", decryptedtext);

    return 0;
}
