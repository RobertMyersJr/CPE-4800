/**
******************************************************************************
* @file des_encrypt.cpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Driver program to encrypt messages 
******************************************************************************
*/
#include "crypt_tools.hpp"
#include "des.hpp"
#include <cstdint>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
int main(int argc, char* argv[]) {

    if(argc < 3) {
        std::printf("ERROR: Need 2 arguments(message, key)\n");
        return 1;
    } else if(argc != 3) {
        std::printf("ERROR: Provided too many arguments\n");
        return 1;
    }
    std::string message = argv[1];
    std::string key_str = argv[2];

    uint64_t key;
    try {
        key = std::stoull(key_str, nullptr, 16);
    }
    catch (const std::exception& e) {
        std::printf("ERROR: Could not parse one of the given argument\n");
        return 1;
    }

    if(message.length() != 8) {
        std::printf("ERROR: message is not 64 bits\n");
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

    uint64_t message_data;
    std::memcpy(&message_data, message.data(), sizeof(message_data));
    
    uint64_t cipher = encrypt(message_data, subkeys);

    std::printf("Original String:  %s\n", message.data());
    std::printf("Original Hex:  %lx\n", message_data);
    std::printf("Cipher:    %lx\n", cipher);

}
