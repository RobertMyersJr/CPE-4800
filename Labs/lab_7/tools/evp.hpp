#pragma once
/**
******************************************************************************
* @file evp.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief evp functions to encrypt and decrypt 
******************************************************************************
*/

#include <span>
#include <string>
#include <vector>
const unsigned char key[33] = "01234567890123456789012345678901";

/**
 * @brief encrypts a message using aes
 *
 * @param plaintext Text being encrypted. Code was lifted and modified from lab example
 * @param plaintext_len the len
 * @param key The key to the message
 * @param iv The iv block
 * @param ciphertext The resulting ciphertext
 * @return 
 */
int encrypt(const unsigned char *plaintext, int plaintext_len, unsigned char*key,
        unsigned char *iv, unsigned char *ciphertext);

/**
 * @brief encrypts a message using aes. Code was lifted and modified from lab example
 *
 * @param ciphertext Text being decrypted
 * @param plaintext_len the len
 * @param key The key to the message
 * @param iv The iv block
 * @param plaintext The resulting plain text
 * @return 
 */
int decrypt(const unsigned char *ciphertext, int plaintext_len, unsigned char*key,
        unsigned char *iv, unsigned char *plaintext);

/**
 * @brief Helper method to quickly encrypt data
 */
std::vector<unsigned char> quick_encrypt(std::string& message);


/**
 * @brief Helper method to quickly decrypt data
 */
std::string quick_decrypt(std::vector<unsigned char>& encrypted_message);

