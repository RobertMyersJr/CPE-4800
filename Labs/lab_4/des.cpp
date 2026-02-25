#include <cstdint>
#include <cstdio>
#include <iostream>
#include <utility>

constexpr int round_table = 48;
constexpr int pc_table_length = 56;
constexpr int c_and_d_length = 28;

constexpr int pc_table[pc_table_length] = {
    57, 49, 41, 33, 25, 17, 9,
    1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27,
    19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4
};

constexpr int round_1_table[pc_table_length] = {
    10,51,34,60,49,17,33,57,2,9,19,42,
    3,35,26,25,44,58,59,1,36,27,18,41,
    22,28,39,54,37,4,47,30,5,53,23,29,
    61,21,38,63,15,20,45,14,13,62,55,31
};

std::pair<uint32_t, uint32_t> apply_pc_1(uint64_t key) {
    uint64_t c_and_d = 0x0;
    uint64_t msb_64 = 0x1ULL << 63;
    uint64_t msb_56 = 0x1ULL << (pc_table_length-1);

    int bit_index = 0;
    int bit_offset = 0;
    for(int i = 0; i < pc_table_length; i++) {
        bool bit = (key&(msb_64 >> (pc_table[bit_index]-1ULL))) > 0;
        c_and_d |= (msb_56 >> (bit_index))*bit;
        bit_index++;
    }
    auto c = c_and_d >> (pc_table_length/2);
    auto d = c_and_d & 0xFFFFFFF;
    std::printf("C %lb\n", c);
    std::printf("D %lb\n", d);
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

int main() {
    auto pair = apply_pc_1(0x133457799BBCDFF1);
    std::printf("c = %x\n", pair.first);
    std::printf("d = %x\n", pair.second);

    auto round_keys = pair;
    for(int i = 1; i <= 1; i++) {
        round_keys = do_round(round_keys, i);
        std::printf("c = %x\n", round_keys.first);
        std::printf("d = %x\n", round_keys.second);
    }

}
