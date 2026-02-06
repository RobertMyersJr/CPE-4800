/**********************************************
*  Filename: mac2.c
*  Description: how to use a MAC and DES secret key
*  Author: Bob Turney
*  Date: 1/6/2025
*  Note: gcc -o mac2 mac2.c -lssl -lcrypto
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/des.h>

void print_hex(const unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    // Message and keys
    const char *message = "Hello, this is a secure message.";
    const char *hmac_key = "supersecretkey";
    const char *des_key = "deskey12"; // DES key must be 8 bytes

    // Buffer for the MAC
    unsigned char mac[EVP_MAX_MD_SIZE];
    unsigned int mac_len;

    // Generate the HMAC using SHA-256
    HMAC(EVP_sha256(), hmac_key, strlen(hmac_key), (unsigned char *)message, strlen(message), mac, &mac_len);

    // DES encryption
    DES_cblock key;
    DES_key_schedule schedule;
    DES_string_to_key(des_key, &key);
    DES_set_key_checked(&key, &schedule);

    // Encrypt the message
    size_t message_len = strlen(message);
    size_t enc_len = ((message_len + 7) / 8) * 8; // DES block size is 8 bytes
    unsigned char enc_message[enc_len];
    DES_cblock ivec = {0}; // Initialization vector

    DES_ncbc_encrypt((unsigned char *)message, enc_message, message_len, &schedule, &ivec, DES_ENCRYPT);

    // Print the encrypted message and its MAC
    printf("Plaintext message: %s\n", message);
    printf("Encrypted Message: ");
    print_hex(enc_message, enc_len);
    printf("MAC: ");
    print_hex(mac, mac_len);

    return 0;
}
