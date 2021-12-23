#include <iostream>
#include "aesesque.h"

int main()
{
    // Settings
    uint64_t ipm[2] = {0x0F79000900090000, 0x00000000DF00D070};
    uint64_t opm[2] = {0x0000000000000000, 0xB70DC0360A5A0000};

    // Variables
    uint64_t word[2];
    uint64_t res[2];

    uint32_t sample_size = 0x20000000; // 2^29
    uint32_t count = 0;
    for (uint32_t i = 0; i < sample_size; i++)
    {
        // Generate random input
        word[0] = generateRandom64Bits();
        word[1] = generateRandom64Bits();

        // Encrypt
        encrypt(word, res);

        // Analyse
        count += getParity(ipm[0] & word[0] ^ ipm[1] & word[1] ^ opm[0] & res[0] ^ opm[1] & res[1]);
    }

    std::cout << count << '/' << sample_size << '\n';

    return 0;
}