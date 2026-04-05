#include <cmath>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>

uint64_t hash_to_u64(const std::string& input) {
    std::hash<std::string> hasher;
    return static_cast<uint64_t>(hasher(input));
}

uint64_t create_verifer(std::string_view salt, std::string_view identify, std::string_view password) {
    std::string hash_input = std::format("{}:{}:{}",salt,identify,password);
    return hash_to_u64(hash_input);
}

uint64_t generate_random_client(uint64_t g, uint64_t a, uint64_t n) {
    double pow = std::pow(g,a);
    return (uint64_t)(pow)%n;
}

uint64_t generate_random_server(uint64_t g, uint64_t a, uint64_t k, uint64_t v, uint64_t n) {
    double pow = std::pow((g),a);
    uint64_t res = k*v + (uint64_t)pow;
    return (uint64_t)(res)%n;
}

uint64_t generate_key_client(uint64_t x, uint64_t B, uint64_t k, uint64_t g, uint64_t a, uint64_t u, uint64_t n) {
    double res_g = std::pow((g),x);

    double base_1 = (B-k*res_g);
    double result = std::pow((uint64_t)base_1, (a+u*x));

    return ((uint64_t)result)%n;
}

uint64_t generate_key_server(uint64_t A, uint64_t v, uint64_t u, uint64_t b, uint64_t n) {
    double res_v = std::pow(v,u);
    double base_1 = (A*res_v);
    double result = std::pow(base_1, b);
    return ((uint64_t)result)%n;
}
