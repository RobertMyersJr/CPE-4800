/**********************************************
*  Filename: aes_cbc_cmac.c
*  Description: How to do openssl for DES
*  Author: Bob Turney
*  Date: 12/27/2024
*  Note: gcc -o aes_cbc aes_cbc.c -lssl -lcrypto
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

    // CMAC calculation
    CMAC_CTX *cmac_ctx = CMAC_CTX_new();
    size_t cmac_len;
    unsigned char cmac_value[16];
    CMAC_Init(cmac_ctx, key, AES_BLOCK_SIZE, EVP_aes_128_cbc(), NULL);
    CMAC_Update(cmac_ctx, ciphertext, plaintext_len);
    CMAC_Final(cmac_ctx, cmac_value, &cmac_len);
    CMAC_CTX_free(cmac_ctx);

    // Print CMAC value
    print_hex("CMAC Value", cmac_value, cmac_len);

    // AES decryption
    AES_set_decrypt_key(key, 128, &dec_key);
    AES_cbc_encrypt(ciphertext, decryptedtext, plaintext_len, &dec_key, iv_dec, AES_DECRYPT);
    decryptedtext[plaintext_len] = '\0'; // Null-terminate the decrypted text

    // Print decrypted text
    printf("Decrypted text: %s\n", decryptedtext);

    return 0;
}
