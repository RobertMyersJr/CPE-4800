/**
 * @file my_hash.cpp
 * @author Robert Myers Jr.
 * @version V1.0
 * @brief Implementation of RFM Hash. See header for more information.
 */
#include "my_hash.hpp"
#include <iomanip>
#include <sstream>
#include <cstring>
#include <vector>

namespace {
    const uint32_t K[32] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967
    };

    uint32_t rotr(uint32_t x, uint32_t n) {
        uint32_t result;
        result = (x >> n) | (x << (32 - n));
        return result;
    }
}

MyHash::MyHash() {
    // Use initial values show here
    state[0] = 0x6a09e667; 
    state[1] = 0xbb67ae85;
    state[2] = 0x3c6ef372; 
    state[3] = 0xa54ff53a;
    state[4] = 0x510e527f; 
    state[5] = 0x9b05688c;
    state[6] = 0x1f83d9ab; 
    state[7] = 0x5be0cd19;
}

void MyHash::transform(std::span<const uint8_t, 64> data) {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t e;
    uint32_t f;
    uint32_t g;
    uint32_t h;
    uint32_t w[16];
    uint32_t temp1;
    uint32_t temp2;

    a = state[0]; 
    b = state[1]; 
    c = state[2]; 
    d = state[3];
    e = state[4]; 
    f = state[5]; 
    g = state[6]; 
    h = state[7];

    // Set up
    for (int i = 0; i < 16; i++) {
        w[i] = (data[i*4] << 24) | (data[i*4+1] << 16) | (data[i*4+2] << 8) | (data[i*4+3]);
    }

    // Do 16 rounds do typical mixing
    for (int i = 0; i < 16; i++) {
        temp1 = h + (rotr(e, 6) ^ rotr(e, 11)) + (f ^ g) + K[i] + w[i];
        temp2 = (rotr(a, 2) ^ rotr(a, 13)) + (b ^ c);
        h = g; 
        g = f; 
        f = e;
        e = d + temp1;
        d = c; 
        c = b; 
        b = a;
        a = temp1 + temp2;
    }

    // do another 16 doing of custom mixing
    for(int i = 16; i < 32; i++) {
        // Make masks to use on values
        uint32_t mask = K[i] & 0xABCDEFAB;
        uint32_t inv_mask = ~K[i] & 0x77777777;
        // Mix up e 
        uint32_t scrambled_e = (e & mask) ^ (f & inv_mask);
        uint32_t temp1_m = h + scrambled_e + rotr(e, 13) + w[i % 16];
        // Mix up a
        uint32_t scrambled_a = (a & inv_mask) ^ (b & mask);
        uint32_t temp2_m = scrambled_a + rotr(a, 17);
        // do typical switching
        h = g;
        g = f;
        f = e;
        e = d + temp1_m;
        d = c;
        c = b;
        b = a;
        a = temp1_m + temp2_m;

    }
    state[0] += a; 
    state[1] += b; 
    state[2] += c; 
    state[3] += d;
    state[4] += e; 
    state[5] += f; 
    state[6] += g; 
    state[7] += h;
}

std::string MyHash::compute(const std::string& message) {
    std::vector<uint8_t> data;
    std::stringstream ss;
    std::string result;

    data.assign(message.begin(), message.end());

    // Append a single 1.
    data.push_back(0x80);

    // Pad with 0 till we have a multiple of 512 bits
    // 64 * 8 = 512
    while ((data.size() % 64) != 0) {
        data.push_back(0x00);
    }

    // Apply a transformation on each byte
    for (size_t i = 0; i < data.size() / 64; i++) {
        std::span<const uint8_t, 64> block(data.data() + (i * 64), 64);
        transform(block);
    }

    // 
    for (int i = 0; i < 8; i++) {
        ss << std::hex << std::setw(8) << std::setfill('0') << state[i];
    }

    result = ss.str();
    return result;
}
