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
        0xfedd9e98,
        0xf6542010,
        0x0121c067,
        0x8dab85eb};

    // Encrypt
    encrypt(word, key, constants, 4);

    // Output result for verification
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

    return 0;
}