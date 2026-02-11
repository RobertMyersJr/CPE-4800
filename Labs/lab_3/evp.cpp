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

void handleErrors(int position) {
    std::cout << "Freak out" << position<< std::endl;
}

std::vector<unsigned char> encrypt_message(std::string message, EVP_PKEY *public_key) {
    assert(public_key != NULL);

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(public_key, NULL);
    if (!ctx) handleErrors(1);
    if (EVP_PKEY_encrypt_init(ctx) <= 0) handleErrors(2);
    size_t encrypted_length;

    if (EVP_PKEY_encrypt (ctx, NULL, &encrypted_length, (unsigned char*)message.data(), message.length() + 1) <= 0) handleErrors(3);

    std::vector<unsigned char> encrypted(encrypted_length);

    if (EVP_PKEY_encrypt (ctx, (unsigned char*)encrypted.data(), &encrypted_length, (unsigned char*)message.data(), message.length() + 1) <= 0) handleErrors(4);
    EVP_PKEY_CTX_free(ctx);

    std::cout << "length = " << encrypted_length << std::endl;
    encrypted.resize(encrypted_length);
    return encrypted;
}

std::string decrypt_message(std::vector<char>& message, EVP_PKEY *private_key) {
    assert(private_key != NULL);

    EVP_PKEY_CTX *ctx_dec = EVP_PKEY_CTX_new(private_key, NULL);
    
    if (!ctx_dec) handleErrors(1);
    if (EVP_PKEY_decrypt_init(ctx_dec) <= 0) handleErrors(2);

    size_t decrypted_length;
    if (EVP_PKEY_decrypt(ctx_dec, NULL, &decrypted_length, (unsigned char*) message.data(), message.size()) <= 0) handleErrors(3);

    
    std::string decrypted_str;
    decrypted_str.resize(decrypted_length);


    if (EVP_PKEY_decrypt(ctx_dec, (unsigned char*)decrypted_str.data(), &decrypted_length, (unsigned char*)message.data(), message.size()) <= 0) handleErrors(4);

    EVP_PKEY_CTX_free(ctx_dec);

    decrypted_str.resize(decrypted_length);
    return decrypted_str;

}
