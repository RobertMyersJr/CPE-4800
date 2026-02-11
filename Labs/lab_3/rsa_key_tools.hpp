#pragma once
#include <openssl/evp.h>

bool open_private_key(EVP_PKEY ** private_key, std::string_view public_key_filepath);
bool open_public_key(EVP_PKEY ** public_key, std::string_view public_key_filepath);
EVP_PKEY *prompt_for_public_key();
