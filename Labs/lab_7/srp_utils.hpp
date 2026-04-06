#ifndef SRP_PROTOCOL_HPP
#define SRP_PROTOCOL_HPP

#include <string>
#include <string_view>
#include <cstdint>
#include <functional>
#include <format>

namespace SRP {
    constexpr uint64_t n = 53;
    constexpr uint64_t g = 2;
    constexpr uint64_t k = 11;

    /**
     * @brief Performs modular exponentiation (base^exp % mod).
     * Prevents overflow by using __uint128_t.
     */
    inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
        uint64_t res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (static_cast<__uint128_t>(res) * base) % mod;
            base = (static_cast<__uint128_t>(base) * base) % mod;
            exp /= 2;
        }
        return res;
    }

    /**
     * @brief Simple hash wrapper. 
     */
    inline uint64_t hash_to_u64(const std::string& input) {
        std::hash<std::string> hasher;
        return static_cast<uint64_t>(hasher(input));
    }


    inline uint64_t create_x(std::string_view salt, std::string_view identity, std::string_view password) {
        std::string hash_input = std::format("{}:{}:{}", salt, identity, password);
        uint64_t x = hash_to_u64(hash_input); 
        return x; 
    }

    /**
     * @brief Generates the password verifier (v)
     */
    inline uint64_t create_verifier(uint64_t x) {
        return mod_pow(g,x,n); 
    }

    /**
     * @brief Client Public Value: A = g^a % n
     */
    inline uint64_t generate_A(uint64_t g, uint64_t a, uint64_t n) {
        return mod_pow(g, a, n);
    }

    /**
     * @brief Server Public Value: B = kv + g^b % n
     */
    inline uint64_t generate_B(uint64_t g, uint64_t b, uint64_t k, uint64_t v, uint64_t n) {
        uint64_t term2 = mod_pow(g, b, n);
        return ((k * v) % n + term2) % n;
    }

    /**
     * @brief Client Shared Secret: S = (B - kg^x)^(a + ux) % n
     */
    inline uint64_t generate_key_client(uint64_t x, uint64_t B, uint64_t k, uint64_t g, uint64_t a, uint64_t u, uint64_t n) {
        uint64_t g_x = mod_pow(g, x, n);
        uint64_t k_gx = (static_cast<__uint128_t>(k) * g_x) % n;
        
        // Ensure base remains positive before modulo
        uint64_t base = (B >= k_gx) ? (B - k_gx) : (n - (k_gx - B) % n);
        uint64_t exp = (a + (static_cast<__uint128_t>(u) * x) % (n - 1)) % (n - 1);
        
        return mod_pow(base, exp, n);
    }

    /**
     * @brief Server Shared Secret: S = (A * v^u)^b % n
     */
    inline uint64_t generate_key_server(uint64_t A, uint64_t v, uint64_t u, uint64_t b, uint64_t n) {
        uint64_t v_u = mod_pow(v, u % (n - 1), n);

        uint64_t base = (static_cast<__uint128_t>(A) * v_u) % n;

        return mod_pow(base, b, n);
    }
}

#endif // SRP_PROTOCOL_HPP
