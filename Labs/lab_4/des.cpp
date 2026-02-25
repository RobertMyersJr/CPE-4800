/**
******************************************************************************
* @file des_encrypt.cpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Helper methods to preform the light weight DES function
* See header for more information 
******************************************************************************
*/
#include "des.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

constexpr int round_table = 48;
constexpr int pc_1_table_length = 56;
constexpr int pc_2_table_length = 48;
constexpr int c_and_d_length = 28;

constexpr int pc_1_table[pc_1_table_length] = {
    57, 49, 41, 33, 25, 17, 9,
    1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27,
    19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4
};

constexpr int pc_2_table[pc_2_table_length] = {
    14,17,11,24,1,5,
    3,28,15,6,21,10,
    23,19,12,4,26,8,
    16,7,27,20,13,2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32
};

std::pair<uint32_t, uint32_t> apply_pc_1(uint64_t key) {
    uint64_t c_and_d = 0x0;
    uint64_t msb_64 = 0x1ULL << 63;
    uint64_t msb_56 = 0x1ULL << (pc_1_table_length-1);

    int bit_index = 0;
    int bit_offset = 0;
    for(int i = 0; i < pc_1_table_length; i++) {
        bool bit = (key&(msb_64 >> (pc_1_table[bit_index]-1ULL))) > 0;
        c_and_d |= (msb_56 >> (bit_index))*bit;
        bit_index++;
    }
    auto c = c_and_d >> (pc_1_table_length/2);
    auto d = c_and_d & 0xFFFFFFF;
    // std::printf("C %lb\n", c);
    // std::printf("D %lb\n", d);
    return {c,d};
}

std::pair<uint32_t, uint32_t> do_round(std::pair<uint32_t, uint32_t> keys, int shift) {
    if(shift == 1 ||
       shift == 2 ||
       shift == 9 ||
       shift == 16) {
        shift = 1;
    } else {
        shift = 2;
    }
    return {
        ((keys.first << shift) | (keys.first >> (c_and_d_length - shift))) & 0xFFFFFFF,
        ((keys.second << shift) | (keys.second >> (c_and_d_length - shift))) & 0xFFFFFFF
    };

}

uint64_t get_subkey(std::pair<uint32_t, uint32_t> keys) {
    uint64_t subkey = 0x0;
    uint64_t key = (((uint64_t)keys.first) << c_and_d_length) | keys.second;
    uint64_t msb_56 = 0x1ULL << (pc_1_table_length-1);
    uint64_t msb_48 = 0x1ULL << 47;
    int bit_index = 0;
    int bit_offset = 0;
    for(int i = 0; i < pc_2_table_length; i++) {
        bool bit = (key&(msb_56 >> (pc_2_table[bit_index]-1ULL))) > 0;
        //std::printf("%d",bit);
        subkey |= (msb_48 >> (bit_index))*bit;
        bit_index++;
    }
    return subkey;
}
