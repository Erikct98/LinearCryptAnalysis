#include "aes.h"
#include "toolbox.h"
#include <random>
#include <algorithm>

uint8_t OPM = 0x01;
uint32_t IPMS[3] = {
    0x62799393,
    0x25648989,
    0x3CAAA9A9};
uint32_t IPMST[4] = {
    0x62253C,
    0x7964AA,
    0x9389A9,
    0x9389A9};

uint32_t MASKS[4] = {
    0xFF000000,
    0x00FF0000,
    0x0000FF00,
    0x000000FF};

uint8_t keyclasses[8] = {0x00, 0x01, 0x02, 0x03, 0x20, 0x21, 0x22, 0x23};

inline uint16_t alteredfourwaysboxinputparity(uint32_t pt, uint8_t kg)
{
    // Linear trail over top-3 sboxes
    uint32_t parity = P32(pt & IPMS[0] & ~MASKS[3]);

    // Majority trail over bottom sbox
    uint32_t p = (((pt & 0xFF) ^ kg) * 0x010101) & IPMST[3];
    p ^= p >> 4;
    p ^= p >> 2;
    p ^= p >> 1;
    p &= 0x010101;
    p = ((p >> 16) | ((p >> 8) & p)) & 1; // A or (B and C)

    // Combine parities and return
    return parity ^ p;
}

void recoverKey(uint64_t sampleSize, uint32_t *key, uint64_t* counters)
{
    // Run test
    uint32_t pt[4];
    uint8_t ipp[8], opp, part;
    for (uint64_t i = 0; i < sampleSize; i++)
    {
        // Generate plaintext
        for (uint16_t j = 0; j < 4; j++)
        {
            pt[j] = i & MASKS[j];
        }

        // Analyse plaintext for all key guesses
        for (uint16_t j = 0; j < 8; j++)
        {
            ipp[j] = alteredfourwaysboxinputparity(i, keyclasses[j]);
        }

        // Encrypt
        encrypt(pt, key, 2);

        // Analyse encryption
        opp = P8((pt[0] >> 24) & OPM);

        // Update counters
        for (uint16_t j = 0; j < 8; j++)
        {
            counters[j] -= ipp[j] ^ opp;
        }
    }
}

void keyRecovery()
{
    // Settings
    uint64_t sampleSize = 0x100000000;
    uint16_t encRounds = 2;

    // Variables
    uint32_t key[4] = {
        0x98A89596,
        0xC47F4EBC,
        0x5C66A2B0,
        0xC411773C
    };
    uint32_t expandedKey[4 * (encRounds + 1)];
    ExpandKey(key, expandedKey, encRounds + 1);

    // Compute parities
    uint64_t counts[8];
    std::fill(&counts[0], &counts[8], sampleSize);
    recoverKey(sampleSize, expandedKey, counts);

    // Print results
    for (uint16_t i = 0; i < 8; i++)
    {
        printResults(&counts[i], 1, sampleSize);
    }
}

int main()
{
    keyRecovery();
}