#include <iostream>
#include <random>
#include "AESesque.h"
#include "toolbox.h"

int main()
{
    // Settings
    uint32_t ipm[4] = {
        0x09990000,
        0x00077000,
        0x000F0F00,
        0x0000DD00};
    uint32_t opm[4] = {
        0x00000A6D,
        0x00000530,
        0x00000A07,
        0x000000CB};
    uint32_t key[4] = {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000};
    uint32_t constants[4] = {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000};
    // uint32_t sample_size = 0x20000000; // 2^29
    uint32_t sample_size = 0x2000; // 2^13
    uint8_t rounds = 4;

    // Randomization
    std::mt19937 mt(0);
    std::uniform_int_distribution<uint32_t> randint(0, 0xFFFFFFFF);

    // Variables
    uint32_t word[4];
    uint32_t count = 0;
    uint8_t parity;

    for (uint32_t i = 0; i < sample_size; i++)
    {
        // Generate random input
        for (uint8_t i = 0; i < 4; i++)
        {
            word[i] = randint(mt);
        }

        // Compute parity input
        parity = 0;
        for (uint8_t i = 0; i < 4; i++)
        {
            parity ^= getParity(word[i] & ipm[i]);
        }

        // Encrypt
        encrypt(word, key, constants, rounds);

        // Compute parity output
        for (uint8_t i = 0; i < 4; i++)
        {
            parity ^= getParity(word[i] & opm[i]);
        }

        // Analyse
        count += parity;
    }

    std::cout << count << '/' << sample_size << '\n';

    return 0;
}