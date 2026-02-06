/**********************************************
*  Filename: mac.c
*  Description: how to use a MAC
*  Author: Robert Myers jr.
*  Date: 1/6/2025
*  Note: gcc -o mac mac.c -lssl -lcrypto
***********************************************/  
#include <stdio.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

void print_hex(const unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    // Message and key
    const char *message = "secret message.";
    const char *key = "12345";

    // Buffer for the MAC
    unsigned char mac[EVP_MAX_MD_SIZE];
    unsigned int mac_len;

    // Generate the HMAC using SHA-256
    HMAC(EVP_sha256(), key, strlen(key), (unsigned char *)message, strlen(message), mac, &mac_len);

    // Print the message and its MAC
    printf("Message: %s\n", message);
    printf("MAC: ");
    print_hex(mac, mac_len);

    return 0;
}

