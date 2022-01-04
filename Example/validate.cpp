#include <iostream>
#include "AESesque.h"

int main()
{
    // Test vector
    uint32_t key[4] = {
        0x01234567,
        0x89abcdef,
        0xfedcba98,
        0x76543210};
    uint32_t constants[4] = {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000};
    uint32_t word[4] = {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000};
    uint32_t output[4] = {
        0x2fbc2062,
        0x88b906de,
        0xbd167818,
        0x929af016};

    // Encrypt
    encrypt(word, key, constants, 4);

    // Check correctness
    bool correct = true;
    for (uint8_t i = 0; i < 4; i++)
    {
        correct &= word[0] == output[0];
    }

    // Output result for verification
    if (correct)
    {
        std::cout << "CORRECT" << std::endl;
    }
    else
    {
        std::cout << "INCORRECT" << std::endl;
        std::cout << "computed: "
                  << word[0] << ' '
                  << word[1] << ' '
                  << word[2] << ' '
                  << word[3] << std::endl;
        std::cout << "expected: "
                  << output[0] << ' '
                  << output[1] << ' '
                  << output[2] << ' '
                  << output[3] << std::endl;
    }

    return 0;
}