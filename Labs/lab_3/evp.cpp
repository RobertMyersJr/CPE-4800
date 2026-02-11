/**
******************************************************************************
* @file evp.cpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Implementation of the evp functions. See header for more information
******************************************************************************
 */
#include <cassert>
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
    std::cout << "Something went wrong during an RSA interaction" << std::endl;
    exit(1);
}

std::vector<unsigned char> encrypt_message(std::string message, EVP_PKEY *public_key) {
    assert(public_key != NULL);
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(public_key, NULL);
    if (!ctx) handleErrors();
    if (EVP_PKEY_encrypt_init(ctx) <= 0) handleErrors();
    size_t encrypted_length;

    if (EVP_PKEY_encrypt (ctx, NULL, &encrypted_length, (unsigned char*)message.data(), message.length() + 1) <= 0) handleErrors();

    std::vector<unsigned char> encrypted(encrypted_length);

    if (EVP_PKEY_encrypt (ctx, (unsigned char*)encrypted.data(), &encrypted_length, (unsigned char*)message.data(), message.length() + 1) <= 0) handleErrors();
    EVP_PKEY_CTX_free(ctx);

    encrypted.resize(encrypted_length);
    return encrypted;
}

std::string decrypt_message(std::vector<char>& message, EVP_PKEY *private_key) {
    assert(private_key != NULL);

    EVP_PKEY_CTX *ctx_dec = EVP_PKEY_CTX_new(private_key, NULL);
    
    if (!ctx_dec) handleErrors();
    if (EVP_PKEY_decrypt_init(ctx_dec) <= 0) handleErrors();

    size_t decrypted_length;
    if (EVP_PKEY_decrypt(ctx_dec, NULL, &decrypted_length, (unsigned char*) message.data(), message.size()) <= 0) handleErrors();

    
    std::string decrypted_str;
    decrypted_str.resize(decrypted_length);


    if (EVP_PKEY_decrypt(ctx_dec, (unsigned char*)decrypted_str.data(), &decrypted_length, (unsigned char*)message.data(), message.size()) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx_dec);

    decrypted_str.resize(decrypted_length);
    if(decrypted_str[decrypted_length - 1] == '\0') {
        decrypted_str.resize(decrypted_length - 1);
    }
    return decrypted_str;

}
