#include <chrono>
#include <cstdlib>
#include <iostream>
#include "AESesque.h"

// Settings
uint32_t key[4] = {
    0x01234567,
    0x89ABCDEF,
    0xFEDCBA98,
    0x76543210};
uint32_t constants[4] = {
    0x47AEB5C6,
    0xE6BA6404,
    0x01E1A321,
    0x4E9CED5D};
uint32_t iterations = 0x1000000;
uint32_t rounds = 4;

void speedtest()
{
    uint32_t word[4] = {0};

    // Perform speedtest
    uint64_t checksum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < iterations; i++)
    {
        encrypt(word, key, constants, rounds);
        checksum += word[0];
    }
    auto stop = std::chrono::high_resolution_clock::now();

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