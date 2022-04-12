#include "toolbox.h"
#include <random>
#include <algorithm>

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

void computeParities(uint64_t *counters, uint16_t nrKeyGuesses, uint32_t *keyguesses, uint32_t *key, uint64_t sampleSize)
{
    // Run test
    uint32_t pt[4], pt_agg;
    uint8_t ipp[nrKeyGuesses], opp, part;
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
        AddRoundKey(pt, key, 0);
        SubBytes(pt);
        ShiftRows(pt);
        MixColumns(pt);
        AddRoundKey(pt, key, 1);
        SubBytes(pt);
        ShiftRows(pt);
        AddRoundKey(pt, key, 2);

        // Analyse encryption
        opp = P8((pt[0] >> 24) & OPM);

        // Update counters
        for (uint16_t j = 0; j < nrKeyGuesses; j++)
        {
            counters[j] -= ipp[j] ^ opp;
        }
    }
}

void printSettings(uint64_t sampleSize, uint16_t encRounds)
{
    std::cout << "\"samplesize\": " << sampleSize << ",\n";
    std::cout << "\"encRounds\": " << encRounds << ",\n";
    std::cout << "\"opm\": \"0x" << std::hex << (uint16_t)OPM << std::dec << "\",\n";
    std::cout << "\"ipms\": [\n";
    for (uint16_t p = 0; p < 3; p++)
    {
        std::cout << "\"0x" << std::hex << (IPMS[p]) << std::dec << "\"";
        if (p < 2) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "],\n";
}

void printResults(uint32_t *keyguesses, uint16_t nrKeyGuesses, uint64_t *counters)
{
    std::cout << "{\n";
    std::cout << "\"key\": \"0x" << std::hex << keyguesses[0] << "\",\n";
    std::cout << "\"counts\":{\n";
    for (uint16_t j = 0; j < nrKeyGuesses; j++)
    {
        std::cout << "\"0x" << std::hex << keyguesses[j] << "\": "
                  << std::dec << counters[j];
        if (j < nrKeyGuesses - 1) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "}}";
}

void monteCarlo()
{
    // Settings
    uint64_t sampleSize = 0x10000000;
    uint16_t trailRounds = 20;
    uint16_t encRounds = 2;
    uint16_t nrKeyGuesses = 17; // of which the first is with the correct key

    // Variables
    uint32_t key[4];
    uint32_t expandedKey[4 * (encRounds + 1)];
    uint32_t keyguesses[nrKeyGuesses];
    uint64_t counters[nrKeyGuesses];
    double corr;
    double corrlog[nrKeyGuesses];

    // Execute test `trailRounds` times
    printSettings(sampleSize, encRounds);
    std::cout << "\"trails\":[\n";
    for (uint16_t r = 0; r < trailRounds; r++)
    {
        // Generate encryption key
        for (uint16_t i = 0; i < 4; i++)
        {
            key[i] = rand_uint32();
        }
        ExpandKey(key, expandedKey, encRounds + 1);

        // Generate key guesses
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

        // Reset counters
        std::fill(counters, counters + nrKeyGuesses, sampleSize);

        // Compute parities
        computeParities(counters, nrKeyGuesses, keyguesses, expandedKey, sampleSize);

        // Print results
        printResults(keyguesses, nrKeyGuesses, counters);
        if (r < trailRounds - 1) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "]\n";
}

int main()
{
    std::cout << "{\n";
    monteCarlo();
    std::cout << "}\n";
}