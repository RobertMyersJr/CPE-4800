#pragma once
#include <string>
#include <openssl/evp.h>
#include <vector>
std::vector<unsigned char> encrypt_message(std::string message, EVP_PKEY *public_key);
std::string decrypt_message(std::vector<char>& message, EVP_PKEY *private_key);
