#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include "simon.h"
#include "encrypt.h"

uint32_t compute_correlation(uint32_t inp_mask, uint32_t out_mask, uint64_t key)
{
    // Constants
    uint16_t subkey;
    uint32_t pt;
    uint32_t count = 0;

    // Generate subkeys
    generateSubKeys(key, &subkey, 1);

    // Compute correlation
    for (uint64_t j = 0; j <= 0xFFFFFFFF; j++)
    {
        pt = (uint32_t) j;
        count += getParity((inp_mask & pt) ^ (out_mask & encrypt(pt, &subkey, 1)));
    }
    return count;
}

int main()
{
    std::srand(time(0));

    // uint32_t inp_mask = 0x00204080, out_mask = 0x40801000; uint8_t enc_rounds = 1; // {5}, {7, 14} -> {7, 14}, {12} // 1 rounds
    // uint32_t inp_mask = 0x00404080, out_mask = 0x40801020; uint8_t enc_rounds = 1; // {6}, {7, 14} -> {7, 14}, {5, 12} // 1 rounds
    uint32_t inp_mask = 0x20404080, out_mask = 0x40801000; uint8_t enc_rounds = 1; // {5, 13}, {7, 14} -> {7, 14}, {12} // 1 rounds

    uint32_t iterations = 10;
    for (uint32_t sample_size = 0; sample_size <= iterations; sample_size ++)
    {
        uint64_t key = getRandomKey();
        uint32_t count = compute_correlation(inp_mask, out_mask, key);
        std::cout << key << ' ' << count << '\n';
    }

    return 0;
}