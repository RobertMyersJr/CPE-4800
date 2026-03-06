/**
 * @file my_hash.hpp
 * @author Robert Myers Jr.
 * @version V1.0
 * @brief A 256-bit hash algorithm inspired by SHA2-256 algorithm
 */
#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <span>

class MyHash {
public:
    MyHash();
    std::string compute(const std::string& message);

private:
    uint32_t state[8]; 
    void transform(std::span<const uint8_t, 64> data);
    void pad(std::vector<uint8_t>& data);
};;
