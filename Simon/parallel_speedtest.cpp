#include <iostream>
#include <chrono>
#include <cstdlib>
#include "omp.h"
#include "simon.h"
#include "toolbox.h"

inline uint64_t computeThings(uint32_t start, uint32_t iterations, uint16_t *subkeys, uint8_t enc_rounds)
{
    uint64_t checksum = 0;
    for (uint32_t i = 0; i < iterations; i ++)
    {
        checksum += encrypt(start + i, subkeys, enc_rounds);
    }
    return checksum;
}

void speedtest()
{
    uint8_t processors = 8;
    uint32_t iterations = 0x8000000;
    uint8_t enc_rounds = 32;
    uint64_t key = 0x1918111009080100;
    uint32_t pt = 0x65656877;
    uint32_t ct = 0xc69be9bb;

    // Generate keys
    uint16_t subkeys[enc_rounds];
    generateSubKeys(key, subkeys, enc_rounds);

    // Perform speedtest
    uint64_t checksums[processors];
    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for (uint8_t P = 0; P < processors; P++)
    {
        checksums[P] = computeThings(P * iterations, iterations, subkeys, enc_rounds);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    uint64_t checksum = 0;
    for (uint8_t P = 0; P < processors; P++)
    {
        checksum += checksums[P];
    }

    // Print results to console
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "checksum: " << checksum << '\n';
    std::cout << "duration (mus): " << duration.count() << '\n'
              << '\n';
}

int main()
{
    speedtest();
    return 0;
}
