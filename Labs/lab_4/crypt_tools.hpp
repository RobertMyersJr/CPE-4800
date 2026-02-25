/**
******************************************************************************
* @file crypt_tools.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Helper methods to encrypt and decrypt.
******************************************************************************
*/
#pragma once
#include <cstdint>
#include <vector>

/**
 * @brief Encrypt a 64 bit message with a list of subkeys
 */
uint64_t encrypt(uint64_t plaintext, const std::vector<uint64_t>& subkeys);

/**
 * @brief Decrypt a 64 bit message with a list of subkeys
 */
uint64_t decrypt(uint64_t ciphertext, const std::vector<uint64_t>& subkeys);
