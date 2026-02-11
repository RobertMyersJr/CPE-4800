#include <iostream>
#include <openssl/evp.h>
#include "rsa_key_tools.hpp"
#include "user_input.hpp"
#include <openssl/pem.h>

bool open_private_key(EVP_PKEY ** private_key, std::string_view public_key_filepath) {
    FILE *fp = fopen(public_key_filepath.data(), "r");

    if (!fp) {
        std::cout << "Failed to open private key\n";
        return false;
    }
    *private_key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    if (*private_key == NULL)
    {
        std::cout << "PEM_read_PrivateKey failed." << std::endl;
    }
    fclose(fp);
    return true;
}

bool open_public_key(EVP_PKEY ** public_key, std::string_view public_key_filepath) {
    FILE *fp = fopen(public_key_filepath.data(), "r");

    if (!fp) {
        std::cout << "Failed to open public key\n";
        return false;
    }
    *public_key = PEM_read_PUBKEY(fp, NULL, NULL, NULL);
    if (*public_key == NULL)
    {
        std::cout << "PEM_read_PUBKEY failed." << std::endl;
    }
    fclose(fp);
    return true;
}

EVP_PKEY *prompt_for_public_key() {
    auto file_path = get_user_input("Path to other user's public key ");
    EVP_PKEY * public_key = EVP_PKEY_new();
    if(!open_public_key(&public_key, file_path)) {
        std::cout << "Failed to acquire public key. Ending program.\n";
        exit(0);
    }
    return public_key;
}
