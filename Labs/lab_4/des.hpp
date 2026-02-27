/**
******************************************************************************
* @file des.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Helper methods to preform the light weight DES function
******************************************************************************
*/
#include <cstdint>
#include <utility>
/**
 * @brief Applys the PC_1 table to the key
 *
 * @param key The key used to get the initial C0 and D0
 */
std::pair<uint32_t, uint32_t> apply_pc_1(uint64_t key);

/**
 * @brief Does a round to shift Cn and C0
 *
 * @param keys The keys that will be shifted
 * @param shift the number of shifts
 */
std::pair<uint32_t, uint32_t> do_round(std::pair<uint32_t, uint32_t> keys, int shift);

/**
 * @brief Generates the subkey with a given Cn and Dn key
 */
uint64_t get_subkey(std::pair<uint32_t, uint32_t> keys);
