#pragma once
#include <string>
#include <openssl/evp.h>
#include <vector>
std::vector<char> encrypt_message(std::string message);
std::string decrypt_message(std::vector<char>& message);
