#include "aes.h"
#include <iostream>
#include <iomanip>

bool TestKeyExpansion()
{
    uint32_t key[4] = {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000
    };
    uint32_t expected[44] = {
        0x00000000, 0x00000000,
        0x00000000, 0x00000000,
        0x62636363, 0x62636363,
        0x62636363, 0x62636363,
        0x9B9898C9, 0xF9FBFBAA,
        0x9B9898C9, 0xF9FBFBAA,
        0x90973450, 0x696CCFFA,
        0xF2F45733, 0x0B0FAC99,
        0xEE06DA7B, 0x876A1581,
        0x759E42B2, 0x7E91EE2B,
        0x7F2E2B88, 0xF8443E09,
        0x8DDA7CBB, 0xF34B9290,
        0xEC614B85, 0x1425758C,
        0x99FF0937, 0x6AB49BA7,
        0x21751787, 0x3550620B,
        0xACAF6B3C, 0xC61BF09B,
        0x0EF90333, 0x3BA96138,
        0x97060A04, 0x511DFA9F,
        0xB1D4D8E2, 0x8A7DB9DA,
        0x1D7BB3DE, 0x4C664941,
        0xB4EF5BCB, 0x3E92E211,
        0x23E951CF, 0x6F8F188E
    };

    uint32_t expanded[44];
    ExpandKey(key, expanded, 11);

    // Check correctness
    bool correct = true;
    for (uint8_t i = 0; i < 44; i++)
    {
        correct &= expected[i] == expanded[i];
    }

    if (correct)
    {
        std::cout << "CORRECT" << std::endl;
        return true;
    }

    // Print result
    std::cout << "INCORRECT" << std::endl;
    std::cout << "computed | expected " << std::endl;
    for (uint32_t i = 0; i < 44; i++)
    {
        std::cout << std::dec << std::setw(2) << std::setfill(' ') << i << ": ";
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << expanded[i];
        std::cout << " | ";
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << expected[i];
        std::cout << std::endl;
    }
    return false;
}

bool TestEncryption()
{
    // Test vector
    uint32_t key[4] = {
        0x01234567,
        0x89abcdef,
        0xfedcba98,
        0x76543210};
    uint32_t pt[4] = {

    };
    uint32_t ct[4] = {

    };

    // Encrypt
    encrypt(pt, key);

    // Check correctness
    bool correct = true;
    for (uint8_t i = 0; i < 4; i++)
    {
        correct &= pt[i] == ct[i];
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
                  << pt[0] << ' '
                  << pt[1] << ' '
                  << pt[2] << ' '
                  << pt[3] << std::endl;
        std::cout << "expected: "
                  << ct[0] << ' '
                  << ct[1] << ' '
                  << ct[2] << ' '
                  << ct[3] << std::endl;
    }
    return false;
}

int main()
{
    TestKeyExpansion();
    return 0;
}