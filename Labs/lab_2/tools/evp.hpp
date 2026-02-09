#pragma once

#include <span>
#include <string>
#include <vector>
const unsigned char key[33] = "01234567890123456789012345678901";

int encrypt(const unsigned char *plaintext, int plaintext_len, unsigned char*key,
        unsigned char *iv, unsigned char *ciphertext);

int decrypt(const unsigned char *ciphertext, int plaintext_len, unsigned char*key,
        unsigned char *iv, unsigned char *plaintext);

std::vector<unsigned char> quick_encrypt(std::string message);
std::string quick_decrypt(std::string message);
