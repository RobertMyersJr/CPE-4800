/**
******************************************************************************
* @file crypt_tools.cpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Helper methods to encrypt and decrypt. See header for more information
******************************************************************************
*/
#include "crypt_tools.hpp"

namespace {
    uint32_t round_function(uint32_t R, uint64_t subkey) {
        // Following similar algorithm here https://www.geeksforgeeks.org/python/feistel-cipher/
        uint32_t key_part = (uint32_t)(subkey & 0xFFFFFFFF);
        uint32_t result = R ^ key_part;
        // cyclic rotate by 3
        return (result << 3) | (result >> 29); 
    }
}
uint64_t encrypt(uint64_t plaintext, const std::vector<uint64_t>& subkeys) {
    uint32_t L = (uint32_t)(plaintext >> 32);
    uint32_t R = (uint32_t)(plaintext & 0xFFFFFFFF);

    for (int i = 0; i < subkeys.size(); i++) {
        uint32_t temp = R;
        R = L ^ round_function(R, subkeys[i]);
        L = temp;
    }

    return ((uint64_t)R << 32) | L;
}

uint64_t decrypt(uint64_t ciphertext, const std::vector<uint64_t>& subkeys) {
    uint32_t L = (uint32_t)(ciphertext >> 32);
    uint32_t R = (uint32_t)(ciphertext & 0xFFFFFFFF);

    for (int i = (int)subkeys.size() - 1; i >= 0; i--) {
        uint32_t temp = R;
        R = L ^ round_function(R, subkeys[i]);
        L = temp;
    }

    return ((uint64_t)R << 32) | L;
}
