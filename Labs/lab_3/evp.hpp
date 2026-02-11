#pragma once
/**
******************************************************************************
* @file evp.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief evp functions to encrypt and decrypt messages using RSA
******************************************************************************
*/

#include <string>
#include <openssl/evp.h>
#include <vector>

/**
 * @brief Encrypts a message using RSA and a given public key
 */
std::vector<unsigned char> encrypt_message(std::string message, EVP_PKEY *public_key);
/**
 * @brief Decrypts a message using RSA and a given private key
 */
std::string decrypt_message(std::vector<char>& message, EVP_PKEY *private_key);
