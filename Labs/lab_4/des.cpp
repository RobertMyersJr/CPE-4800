#include <cstdint>
#include <cstdio>
#include <utility>
constexpr int pc_table_length = 56;
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

std::pair<uint32_t, uint32_t> get_subkey(uint64_t key, uint64_t ) {
    uint64_t c_and_d = 0x0;
    uint64_t msb_64 = 0x1ULL << 63;
    uint64_t msb_56 = 0x1ULL << (pc_table_length-1);


    int bit_index = 0;
    for(int i = 0; i < 64; i++) {
        if(i != 0 && i%8 == 0){
            continue;
        }
        bool bit = (key&(msb_64 >> pc_table[bit_index])) > 0;
        std::printf("%d",bit);
        c_and_d |= (msb_56 >> (bit_index))*bit;
        bit_index++;
    }
    std::printf("\n");
    auto c = c_and_d >> (pc_table_length/2);
    auto d = c_and_d & 0xFFFFFFF;
    return {c,d};
}


int main() {
    auto pair = get_subkey(0x133457799BBCDFF1, 3);
    std::printf("c = %x\n", pair.first);
    std::printf("d = %x\n", pair.second);

}
