/**
******************************************************************************
* @file evp.cpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Implementation of the evp functions. See header for more information 
******************************************************************************
*/
#include "evp.hpp"
#include <array>
#include <cstring>
#include <iostream>
#include <numbers>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <ostream>
#include <span>
#include <string>
#include <strings.h>
#include <vector>

constexpr int max = 1024;

int encrypt (const unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if (!(ctx = EVP_CIPHER_CTX_new())) return -1;
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) return -1;
    if (EVP_EncryptUpdate (ctx, ciphertext, &len, plaintext, plaintext_len) != 1) return -1; 

    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) return -1; 

    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}
int decrypt (const unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if (!(ctx = EVP_CIPHER_CTX_new())) return -1;

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) return -1;

    if (EVP_DecryptUpdate (ctx, plaintext, &len, ciphertext, ciphertext_len) != 1) return -1; 

    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) return -1;
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}
std::vector<unsigned char> quick_encrypt(std::string& plaintext) {
    std::vector<unsigned char> buffer(EVP_MAX_IV_LENGTH + plaintext.length() + EVP_MAX_BLOCK_LENGTH);

    RAND_bytes(buffer.data(), EVP_MAX_IV_LENGTH);

    int cipher_len = encrypt(
        (unsigned char*)plaintext.data(), (int)plaintext.length(),
        (unsigned char*)key,
        buffer.data(), 
        buffer.data() + EVP_MAX_IV_LENGTH 
    );

    buffer.resize(EVP_MAX_IV_LENGTH + cipher_len);
    return buffer;
}

std::string quick_decrypt(std::vector<unsigned char>& encrypted_message) {
    if (encrypted_message.size() < EVP_MAX_IV_LENGTH) return "";

    int ciphertext_len = (int)encrypted_message.size() - EVP_MAX_IV_LENGTH;
    std::vector<unsigned char> plain_buf(ciphertext_len + EVP_MAX_BLOCK_LENGTH);

    int plain_len = decrypt(
        encrypted_message.data() + EVP_MAX_IV_LENGTH,
        ciphertext_len,
        (unsigned char*)key,
        encrypted_message.data(),
        plain_buf.data()
    );

    if (plain_len <= 0) return "DECRYPTION_FAILED";

    return std::string((char*)plain_buf.data(), plain_len);
}
