#include "aes.h"
#include "toolbox.h"
#include <random>

uint8_t OPM = 0x01;
uint32_t IPMS[3] = {
    0x62799393,
    0x25648989,
    0x3CAAA9A9};

inline uint16_t sboxInputParity(uint8_t pt, uint8_t *ipms)
{
    uint8_t P0 = P8(ipms[0] & pt);
    uint8_t P1 = P8(ipms[1] & pt);
    uint8_t P2 = P8(ipms[2] & pt);
    return (5 - 2 * (P0 + P1 + P2 + (P0 ^ P1) + (P0 + P2))) < 0;
}

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

void monteCarlo()
{
    // Settings
    uint64_t sampleSize = 0x20000000;
    uint16_t rounds = 2;
    uint16_t nrKeyGuesses = 17; // of which the first is with the correct key

    // Generate encryption key
    uint32_t key[4];
    std::cout << std::hex;
    for (uint16_t i = 0; i < 4; i++)
    {
        key[i] = rand_uint32();
        std::cout << key[i] << std::endl;
    }
    std::cout << std::dec;
    uint32_t expandedKey[4 * (rounds + 1)];
    ExpandKey(key, expandedKey, rounds + 1);

    // Generate key guesses
    uint32_t keyguesses[nrKeyGuesses];
    // Set first key equal to correct key
    keyguesses[0] = 0;
    for (uint16_t i = 0; i < 4; i++)
    {
        keyguesses[0] ^= key[i] & (0xFF << (8 * (3 - i)));
    }
    // Choose other keys at random
    for (uint16_t i = 1; i < nrKeyGuesses; i++)
    {
        keyguesses[i] = rand_uint32();
    }

    // Run test
    uint32_t pt[4], pt_agg;
    uint8_t ipp[nrKeyGuesses], opp, part;
    int64_t counters[nrKeyGuesses];
    std::fill(counters, counters + nrKeyGuesses, sampleSize);
    for (uint64_t i = 0; i < sampleSize; i++)
    {
        // Generate plaintext
        pt_agg = 0;
        for (uint16_t j = 0; j < 4; j++)
        {
            pt[j] = rand_uint32();
            pt_agg ^= pt[j] & (0xFF << (8 * (3 - j)));
        }

        // Analyse plaintext for all key guesses
        for (uint16_t j = 0; j < nrKeyGuesses; j++)
        {
            ipp[j] = fourwaysboxinputparity(pt_agg ^ keyguesses[j]);
        }

        // Encrypt
        AddRoundKey(pt, expandedKey, 0);
        SubBytes(pt);
        ShiftRows(pt);
        MixColumns(pt);
        AddRoundKey(pt, expandedKey, 1);
        SubBytes(pt);
        ShiftRows(pt);
        AddRoundKey(pt, expandedKey, 2);

        // Analyse encryption
        opp = P8((pt[0] >> 24) & OPM);

        // Update counters
        for (uint16_t j = 0; j < nrKeyGuesses; j++)
        {
            counters[j] -= ipp[j] ^ opp;
        }
    }

    // Analyse results
    double corr, corrlog;
    for (uint16_t j = 0; j < nrKeyGuesses; j++)
    {
        corr = 2 * (counters[j] / (double)sampleSize) - 1;
        float corrlog = std::log2(std::abs(corr));
        std::cout << "idx " << j
                  << ", cnt=" << counters[j]
                  << ", log=" << corrlog
                  << std::endl;
    }
}

int main()
{
    monteCarlo();
}