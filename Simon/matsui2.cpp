#include <chrono>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "simon.h"
#include "toolbox.h"

// 5-round approximation SIMON32, used together with 7 encryption to achieve key.
uint32_t computeNonLinearParity(uint32_t ct, uint16_t key)
{
    const uint16_t X6R12_part = ct >> 11 & ct >> 4;                             // X7R[11] * X7R[ 4]
    const uint32_t X6R01 = ct >> 17 ^ key >> 0 ^ ct >> 15 ^ ct >> 0 & ct >> 9;  // X7L[1] + k7[1] + X7R[15] + X7R[0] * X7R[9]
    const uint32_t X6R10 = ct >> 26 ^ key >> 2 ^ ct >> 8 ^ ct >> 9 & ct >> 2;   // X7L[10] + k7[10] + X7R[8] + X7R[9] * X7R[2]
    const uint32_t X6R13 = ct >> 29 ^ key >> 3 ^ ct >> 11 ^ ct >> 12 & ct >> 5; // X7L[13] + k7[13] + X7R[11] + X7R[12] * X7R[5]
    const uint32_t X6R06 = ct >> 22 ^ key >> 1 ^ ct >> 4 ^ ct >> 5 & ct >> 14;  // X7L[6] + k7[6] + X7R[4] + X7R[5] * X7R[14]
    return (X6R12_part ^ X6R01 & X6R10 ^ X6R13 & X6R06) & 1;
}

void attack(uint32_t rounds, uint32_t sample_size, uint64_t key)
{
    // Settings
    const uint32_t nr_keys = 16;
    const uint32_t ipm = 0x40040001;
    const uint32_t opm = 0x10004404;

    // Setup
    uint16_t subkeys[rounds];
    generateSubKeys(key, subkeys, rounds);

    // Perform attack
    int32_t counts[nr_keys] = {0};
    uint32_t pt, ct, linear_parity;
    for (uint32_t i = 0; i < sample_size; i++)
    {
        pt = rand_uint32();
        ct = encrypt(pt, subkeys, rounds);
        linear_parity = getParity(pt & ipm ^ ct & opm);

        for (uint32_t j = 0; j < nr_keys; j++)
        {
            counts[j] += (linear_parity ^ computeNonLinearParity(ct, j)) & 1;
        }
    }

    for (uint32_t i = 0; i < nr_keys; i++)
    {
        counts[i] = abs(counts[i] - (int32_t)(sample_size >> 1));
    }

    // Output result
    int32_t *maxval = std::max_element(std::begin(counts), std::end(counts));
    uint32_t masked = subkeys[6] & 0x2442;
    uint32_t correct_index = (masked >> 10 & 0x8) ^ (masked >> 8 & 0x4) ^ (masked >> 5 & 0x2) ^ (masked >> 1 & 0x1);

    if (counts[correct_index] != *maxval)
    {
        std::cout << "INCORRECT! GUESSED " << *maxval << ", WAS " << correct_index << std::endl;
        for (uint32_t i = 0; i < nr_keys; i++)
        {
            std::cout << i << "=" << counts[i] << ", ";
        }
        std::cout << std::endl
                  << std::endl;
    }
}

int main()
{
    // Settings
    const uint32_t nr_iterations = 0x4000;
    const uint32_t rounds = 7;
    const uint32_t sample_size = 0x4000;

    for (uint16_t i = 0; i < nr_iterations; i++)
    {
        attack(rounds, sample_size, rand_uint64());
    }

    return 0;
}