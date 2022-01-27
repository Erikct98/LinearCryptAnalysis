#include "aes.h"
#include "toolbox.h"
#include <iostream>
#include <iomanip>

#define ROUNDS 10

void TestKeyExpansion()
{
    uint32_t key[4] = {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000};
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
        0x23E951CF, 0x6F8F188E};

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
    }
    else
    {
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
    }
}

void TestEncryption()
{
    // Test vector
    uint32_t key[4] = {
        0x2b7e1516,
        0x28aed2a6,
        0xabf71588,
        0x09cf4f3c};
    uint32_t pt[4] = {
        0x3243f6a8,
        0x885a308d,
        0x313198a2,
        0xe0370734};
    uint32_t ct[4] = {
        0x3925841d,
        0x02dc09fb,
        0xdc118597,
        0x196a0b32};

    // Expand key
    uint32_t ExpandedKey[4 * (ROUNDS + 1)];
    ExpandKey(key, ExpandedKey, ROUNDS + 1);

    // Encrypt
    encrypt(pt, ExpandedKey, ROUNDS);

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
}

void TestEncryptionV2()
{
    uint32_t pt[4] = {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000};
    uint32_t key[4] = {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000};
    uint32_t ct[4] = {
        0x66E94BD4,
        0xEF8A2C3B,
        0x884CFA59,
        0xCA342B2E};

    // Expand key
    uint32_t ExpandedKey[4 * (ROUNDS + 1)];
    ExpandKey(key, ExpandedKey, ROUNDS + 1);

    encrypt(pt, ExpandedKey, ROUNDS);

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
    }

    ct[0] = 0xF795BD4A;
    ct[1] = 0x52E29ED7;
    ct[2] = 0x13D313FA;
    ct[3] = 0x20E98DBC;

    encrypt(pt, ExpandedKey, ROUNDS);

    correct = true;
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
    }
}

int main()
{
    TestKeyExpansion();
    TestEncryption();
    TestEncryptionV2();
    return 0;
}