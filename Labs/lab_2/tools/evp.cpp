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

    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) - 1) return -1; 

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

std::vector<unsigned char> quick_encrypt(std::string message) {
    size_t max_out_len = message.length() + EVP_MAX_BLOCK_LENGTH;
    std::vector<unsigned char> buffer(EVP_MAX_IV_LENGTH + max_out_len);

    // 2. Generate IV directly into the start of the vector
    unsigned char* iv_ptr = buffer.data();
    if (RAND_bytes(iv_ptr, EVP_MAX_IV_LENGTH) != 1) {
        throw std::runtime_error("Random generation failed");
    }

    // 3. Encrypt directly into the vector, offset by IV length
    unsigned char* ciphertext_ptr = buffer.data() + EVP_MAX_IV_LENGTH;
    
    int encrypted_len = encrypt(
        reinterpret_cast<const unsigned char*>(message.data()), 
        message.length(), 
        (unsigned char*)key, 
        iv_ptr, 
        ciphertext_ptr
    );

    buffer.resize(EVP_MAX_IV_LENGTH + encrypted_len);
    return buffer;
}

std::span<unsigned char> quick_decrypt(std::string message) {
    unsigned char buffer[max];
    unsigned char decrypted[max];
    unsigned char message_out_[max];
    unsigned char iv[max];

    std::cout << "1" << std::endl;
    memcpy(iv, message.c_str(), EVP_MAX_IV_LENGTH);
    int decrypted_len = decrypt(message_out_ + EVP_MAX_IV_LENGTH, message.length() - EVP_MAX_KEY_LENGTH, (unsigned char *)key, iv, decrypted);
    std::cout << "2" << std::endl;
    decrypted[decrypted_len] = '\0';
    std::cout << "3" << std::endl;
    return std::string((char*)decrypted);
}
