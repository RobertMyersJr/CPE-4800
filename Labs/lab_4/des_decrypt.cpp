/**
 ******************************************************************************
 * @file des_decrypt.cpp
 * @author Robert Myers Jr.
 * @version V1.0
 * @brief Driver program to decrypt messages 
 ******************************************************************************
 */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <string>
#include <vector>
#include "des.hpp"
#include "crypt_tools.hpp"
int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::printf("ERROR: Need 2 arguments(message, key)\n");
        return 1;
    } else if(argc != 3) {
        std::printf("ERROR: Provided too many arguments\n");
        return 1;
    }

    std::string hex_str = argv[1];
    std::string key_str = argv[2];

    uint64_t cipher;
    uint64_t key;
    try {
        cipher = std::stoull(hex_str, nullptr, 16);
        key = std::stoull(key_str, nullptr, 16);
    }
    catch (const std::exception& e) {
        std::printf("ERROR: Could parse not one of the given argument\n");
        return 1;
    }

    auto pair = apply_pc_1(key);
    std::vector<uint64_t> subkeys;

    auto round_keys = pair;
    for(int i = 1; i <= 1000; i++) {
        round_keys = do_round(round_keys, i);
        auto key = get_subkey(round_keys);
        subkeys.push_back(key);
    }

    uint64_t plain = decrypt(cipher, subkeys);
    std::string message_decrypted;
    message_decrypted.resize(sizeof(message_decrypted));
    std::memcpy(message_decrypted.data(), &plain, sizeof(plain));
    std::printf("Decrypted Hex: %lx\n", plain);
    std::printf("Decrypted String: %s\n", message_decrypted.data());

}
