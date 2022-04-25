#include "aes.h"
#include "toolbox.h"
#include <random>
#include <algorithm>

uint8_t OPM = 0x01;
uint32_t IPMS[3] = {
    0x62799393,
    0x25648989,
    0x3CAAA9A9};

uint32_t MASKS[4] = {
    0xFF000000,
    0x00FF0000,
    0x0000FF00,
    0x000000FF};

inline uint16_t fourwaysboxinputparity(uint32_t pt)
{
    uint32_t P[3], parity;
    for (uint16_t i = 0; i < 3; i++)
    {
        P[i] = pt & IPMS[i];

        // Compute partial parity
        P[i] ^= P[i] >> 4;
        P[i] ^= P[i] >> 2;
        P[i] ^= P[i] >> 1;
        P[i] &= 0x01010101;
    }
    // Compute majority vote
    parity = (P[0] + P[1] + P[2] + (P[0] ^ P[1]) + (P[0] ^ P[2]));
    parity ^= 0x05050505;
    parity &= parity >> 2;

    // Compute actual parity
    parity ^= parity >> 16;
    return (parity ^ (parity >> 8)) & 0x1;
}

uint64_t computeParities(uint64_t sampleSize, uint32_t keyguess, uint32_t *key)
{
    // Run test
    uint32_t pt[4];

    uint8_t ipp, opp, part;
    uint64_t counter = sampleSize;
    for (uint64_t i = 0; i < sampleSize; i++)
    {
        // Generate plaintext
        for (uint16_t j = 0; j < 4; j++)
        {
            pt[j] = i & MASKS[j];
        }

        // Analyse plaintext for all key guesses
        ipp = fourwaysboxinputparity(i ^ keyguess);

        // Encrypt
        encrypt(pt, key, 2);

        // Analyse encryption
        opp = P8((pt[0] >> 24) & OPM);

        // Update counters
        counter -= ipp ^ opp;
    }
    return counter;
}

void monteCarlo()
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
    uint32_t keyguess = 0xB2FC3992;
    uint32_t expandedKey[4 * (encRounds + 1)];

    // Execute test `trailRounds` times
    // Generate encryption key
    ExpandKey(key, expandedKey, encRounds + 1);

    // Compute parities
    uint64_t count = computeParities(sampleSize, keyguess, expandedKey);

    // Print results
    printResults(&count, 1, sampleSize);
}

int main()
{
    monteCarlo();
}