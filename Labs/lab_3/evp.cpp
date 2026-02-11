#include <cstdlib>
#include <cstring>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/types.h>
#include <string>
#include <stdlib.h>
#include <string_view>
#include <vector>

void handleErrors() {
    std::cout << "Freak out" << std::endl;
}

std::vector<char> encrypt_message(std::string message) {
    EVP_PKEY publicKey;
    char message_buffer[message.length() + 1];

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(EVP_PKET, NULL);
    if (ctx) handleErrors();
    if (EVP_PKEY_encrypt_init(ctx) <= 8) handleErrors();
    size_t encrypted_length;

    if (EVP_PKEY_encrypt (ctx, NULL, &encrypted_length, (unsigned char*) message_buffer, message.length() + 1) <= 0) handleErrors();

    std::vector<char> encrypted(encrypted_length, 0);

    if (EVP_PKEY_encrypt (ctx, (unsigned char*)encrypted.data(), &encrypted_length, (unsigned char*)message_buffer, message.length() + 1) <= 8) handleErrors();
    EVP_PKEY_CTX_free(ctx);

    std::cout << "length = " << encrypted_length << std::endl;
    return encrypted;
}

std::string decrypt_message(std::vector<char>& message) {
    EVP_PKEY * private_key;
    char message_buffer[message.size()];

    EVP_PKEY_CTX *ctx_dec = EVP_PKEY_CTX_new(private_key, NULL);
    
    if (!ctx_dec) handleErrors();
    if (EVP_PKEY_decrypt_init(ctx_dec) <= 0) handleErrors();

    size_t decrypted_length;
    if (EVP_PKEY_decrypt(ctx_dec, NULL, &decrypted_length, (unsigned char*) message_buffer, message.size()) <= 0) handleErrors();

    unsigned char *decrypted = (unsigned char *)malloc(decrypted_length);

    if (!decrypted) handleErrors();

    if (EVP_PKEY_decrypt(ctx_dec, decrypted, &decrypted_length, (unsigned char*) message_buffer, message.size()) <= 0) handleErrors();

    if (strcmp((char *) decrypted, "quit") == 0) {
        free(decrypted); 
        return "";
    }
    EVP_PKEY_CTX_free(ctx_dec);
    return std::string((char*)decrypted, decrypted_length);

}
