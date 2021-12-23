#include <iostream>
#include <chrono>
#include <cstdlib>
#include "simon.h"
#include "encrypt.h"

void speedtest()
{
    uint32_t iterations = 0x1000000;
    uint8_t enc_rounds = 32;
    uint64_t key = 0x1918111009080100;
    uint32_t pt = 0x65656877;
    uint32_t ct = 0xc69be9bb;

    // Generate keys
    uint16_t subkeys[enc_rounds];
    generateSubKeys(key, subkeys, enc_rounds);

    // Perform speedtest
    uint64_t checksum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < iterations; i++)
    {
        checksum += encrypt(i, subkeys, enc_rounds);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    // Print results to console
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    print_name();
    std::cout << "checksum: " << checksum << '\n';
    std::cout << "duration (mus): " << duration.count() << '\n'
              << '\n';
}

int main()
{
    speedtest();
    return 0;
}