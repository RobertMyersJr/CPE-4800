#pragma once
/**
******************************************************************************
* @file rsa_key_tools.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief helper methods to help with getting private and public keys.
******************************************************************************
*/
#include <openssl/evp.h>
#include <string_view>

/**
 * @brief Opens a private key given it's file path
 * @return Flag that indicates if the action was valid
 */
bool open_private_key(EVP_PKEY ** private_key, std::string_view public_key_filepath);

/**
 * @brief Opens a public key given it's file path
 * @return Flag that indicates if the action was valid
 */
bool open_public_key(EVP_PKEY ** public_key, std::string_view public_key_filepath);


/**
 * @brief Prompts the user for a public key file. Will exit if program fails
 *
 * @return The public key found
 */
EVP_PKEY *prompt_for_public_key();
