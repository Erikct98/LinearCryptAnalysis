#include "aes.h"
#include "toolbox.h"

uint8_t ipm_masks[768] = {
    0x00, 0x00, 0x00, 0xA3, 0x2D, 0xC4, 0xFD, 0xB0, 0x6A, 0x4F, 0x9A, 0xFB,
    0x6A, 0x80, 0xB0, 0x3A, 0x6C, 0x89, 0xA7, 0x6A, 0xDA, 0x63, 0x71, 0x90,
    0x35, 0x40, 0xD8, 0xDE, 0xD0, 0x70, 0xBA, 0xA4, 0x8C, 0x6D, 0x62, 0x51,
    0xD9, 0xB6, 0xA6, 0x39, 0x3E, 0xD6, 0x31, 0x38, 0xC8, 0x05, 0x08, 0xCA,
    0x64, 0x22, 0xAA, 0xB0, 0x97, 0x30, 0xEF, 0xE8, 0xB8, 0xEA, 0xCD, 0xB0,
    0xDD, 0x1B, 0x52, 0xCD, 0x30, 0xB0, 0x62, 0x25, 0x3C, 0x17, 0x5A, 0xEA,
    0x9B, 0xD5, 0xD4, 0x77, 0x71, 0x63, 0x9C, 0x03, 0x6B, 0xB5, 0x9A, 0xFA,
    0x18, 0x04, 0xE4, 0x16, 0x48, 0x62, 0x96, 0x12, 0x63, 0x69, 0xE8, 0x86,
    0x04, 0xB9, 0xE0, 0x27, 0x5A, 0x80, 0xD8, 0x13, 0x40, 0xDA, 0xCD, 0x30,
    0xF7, 0x03, 0xAB, 0x80, 0x97, 0xDA, 0x1E, 0x99, 0x92, 0x97, 0x30, 0xDA,
    0x76, 0x3A, 0xB3, 0x11, 0x9E, 0xCC, 0x66, 0x98, 0xD8, 0xF1, 0xCE, 0x64,
    0xB1, 0x1E, 0x92, 0x93, 0x89, 0xA9, 0x55, 0xDB, 0xA3, 0x1F, 0x32, 0xA3,
    0x4D, 0x6A, 0xEA, 0x0E, 0x70, 0xA2, 0x30, 0x27, 0x7D, 0x6F, 0x10, 0xE2,
    0x7D, 0x6A, 0x80, 0xCA, 0x08, 0x2C, 0x5A, 0xCD, 0xFD, 0x38, 0xB2, 0xC1,
    0x0C, 0x02, 0x72, 0x7A, 0x38, 0xC1, 0x21, 0xC2, 0xCA, 0x79, 0x64, 0xAA,
    0x45, 0x41, 0xE0, 0x85, 0x52, 0x58, 0x34, 0xC0, 0xC3, 0x44, 0x10, 0x2B,
    0x02, 0x70, 0xDC, 0x3C, 0x33, 0x19, 0x09, 0x7A, 0xC8, 0xE4, 0xF8, 0x59,
    0xC3, 0xC0, 0x9C, 0xAA, 0x1D, 0xEC, 0xED, 0x8B, 0x98, 0x92, 0x15, 0x3D,
    0xFB, 0x01, 0xD5, 0x7C, 0xA2, 0xD2, 0x47, 0x33, 0x7B, 0x60, 0x4E, 0x9B,
    0x8A, 0x09, 0xC1, 0x28, 0x23, 0x99, 0xCB, 0xED, 0x98, 0x43, 0x85, 0x94,
    0xD5, 0x01, 0x61, 0xA9, 0x1A, 0x76, 0x08, 0xCF, 0xE6, 0xFA, 0xD5, 0x61,
    0xB3, 0x4C, 0x6C, 0x95, 0xAA, 0x79, 0x10, 0x3B, 0xE2, 0x74, 0x19, 0x48,
    0x91, 0x4A, 0xC4, 0xC4, 0x4A, 0x78, 0xC9, 0x44, 0x54, 0xC6, 0x0A, 0x52,
    0x2A, 0x6D, 0x51, 0x2C, 0x08, 0xEB, 0x37, 0xAB, 0x9F, 0xB9, 0xF8, 0xE4,
    0x03, 0x5C, 0x68, 0xA4, 0xB1, 0x92, 0x07, 0xB8, 0xD1, 0x41, 0x18, 0xE4,
    0x98, 0x13, 0xBE, 0xB6, 0x10, 0xF2, 0x52, 0x58, 0x49, 0xEB, 0x05, 0x08,
    0x29, 0x2C, 0x24, 0x8D, 0x10, 0x3B, 0x65, 0xE1, 0xF3, 0x99, 0x92, 0xA4,
    0xAD, 0x53, 0xCB, 0xF3, 0xE1, 0x14, 0x5B, 0x79, 0x88, 0x57, 0x50, 0xD1,
    0x86, 0x81, 0x39, 0x70, 0xA0, 0xDC, 0xCC, 0x49, 0x52, 0x12, 0x90, 0xF5,
    0x14, 0x12, 0x82, 0xC1, 0x42, 0xB2, 0xBC, 0x32, 0xE9, 0x8E, 0xE9, 0x78,
    0xA2, 0xA0, 0x70, 0x20, 0x76, 0xE5, 0xAB, 0xA8, 0x68, 0xF0, 0x4B, 0x7A,
    0x56, 0xA9, 0xB3, 0x6E, 0x39, 0x50, 0x22, 0x88, 0x95, 0xA1, 0x18, 0xFC,
    0x81, 0xB8, 0x6E, 0x9F, 0xAB, 0x68, 0x75, 0x13, 0xD8, 0x0D, 0x21, 0xCA,
    0x32, 0x55, 0x91, 0xDB, 0xC4, 0x78, 0x72, 0x7C, 0xAC, 0xD3, 0xF1, 0x64,
    0x61, 0x60, 0x4E, 0x5D, 0x18, 0x41, 0x8B, 0xAD, 0x75, 0x2E, 0x9B, 0xFA,
    0xEE, 0xE3, 0x29, 0x50, 0x39, 0xD6, 0x49, 0x0A, 0x9E, 0xD2, 0xD0, 0x0C,
    0x4E, 0x01, 0xFB, 0xA6, 0xE2, 0xC9, 0x3E, 0x69, 0xD1, 0x2F, 0x60, 0x9B,
    0x23, 0x3D, 0x99, 0xA5, 0xA1, 0x18, 0xBB, 0x31, 0x38, 0x3F, 0x79, 0x5B,
    0x4B, 0x09, 0x31, 0xDF, 0x1A, 0x20, 0x94, 0x11, 0x58, 0x5F, 0x68, 0x9F,
    0xE5, 0xA9, 0x93, 0xF2, 0xC9, 0x54, 0x0B, 0xA4, 0xB1, 0xBF, 0x81, 0xD1,
    0xFE, 0xD8, 0xCB, 0xC8, 0x38, 0xB2, 0x54, 0x3B, 0xD9, 0x06, 0x63, 0x90,
    0x84, 0x63, 0xF3, 0xC0, 0xF7, 0x9C, 0x51, 0x16, 0x62, 0x9E, 0x1B, 0x43,
    0x53, 0x35, 0xED, 0x59, 0xF8, 0xFC, 0x4A, 0x55, 0xBC, 0xE9, 0x32, 0x78,
    0x88, 0x1D, 0x79, 0x19, 0x2A, 0x48, 0x9D, 0x2B, 0xD9, 0x73, 0xF0, 0xC8,
    0x48, 0x6D, 0x2A, 0xC5, 0x3A, 0x93, 0x36, 0x28, 0xB1, 0xD7, 0x0A, 0x11,
    0x1A, 0x20, 0x6C, 0xFC, 0xF8, 0xB9, 0xE3, 0x05, 0x29, 0xAE, 0xA2, 0xA0,
    0x15, 0x23, 0x28, 0x24, 0x21, 0xEB, 0x82, 0x84, 0x65, 0x7B, 0x74, 0x6D,
    0xEC, 0xD3, 0x5B, 0xE1, 0x14, 0x96, 0xDC, 0x72, 0xA0, 0xA8, 0x9C, 0x6B,
    0x01, 0x2E, 0xB4, 0x1B, 0x58, 0x94, 0xB7, 0x88, 0xF1, 0xB4, 0x9A, 0x60,
    0x26, 0x13, 0x53, 0x5C, 0x6B, 0xA8, 0xA0, 0xAC, 0xD2, 0xE2, 0xC9, 0x9D,
    0x4C, 0x89, 0x93, 0x71, 0x14, 0x82, 0x1C, 0x59, 0xE0, 0x7F, 0xF2, 0xC9,
    0xBE, 0x8B, 0x40, 0x89, 0x20, 0x56, 0xF5, 0x71, 0x82, 0x7E, 0x02, 0xA2,
    0x90, 0xF5, 0x71, 0xC7, 0x21, 0xE3, 0x46, 0x5B, 0x88, 0x3D, 0x0B, 0xBA,
    0xB2, 0xF0, 0xF9, 0x0A, 0x11, 0x94, 0xBD, 0xA1, 0xE0, 0xE7, 0xE1, 0x90,
    0x9A, 0x60, 0x61, 0xAF, 0xBA, 0x23, 0xF9, 0xF0, 0x8A, 0xD4, 0x9A, 0x61,
    0x2D, 0x91, 0xE9, 0xF6, 0xE9, 0xDB, 0xC2, 0x29, 0x2C, 0x6B, 0x34, 0xF4,
    0x6C, 0x20, 0x89, 0x68, 0x34, 0xA8, 0xB8, 0x50, 0xD6, 0x83, 0x31, 0xC8,
    0xE6, 0xC2, 0x29, 0xFF, 0x1A, 0x93, 0x13, 0x40, 0xAD, 0x33, 0x25, 0x48,
    0x8F, 0x43, 0x49, 0x1D, 0xD3, 0xF1, 0xE0, 0x41, 0x59, 0xCF, 0xC2, 0x21,
    0x5E, 0x19, 0x2A, 0x42, 0x31, 0xF9, 0x40, 0x8B, 0xED, 0xD6, 0xE8, 0x69,
    0xD1, 0x50, 0xB8, 0x25, 0x2A, 0x7B, 0x78, 0x67, 0x32, 0x67, 0x4A, 0x91,
    0x3B, 0x9D, 0xD9, 0x87, 0x0B, 0x28, 0xF8, 0x45, 0x5D, 0xAC, 0xD0, 0xD2,
    0x2B, 0x54, 0xD9, 0xCE, 0x22, 0x5B, 0x0F, 0x19, 0x51, 0xF4, 0xC3, 0xAB,
    0x58, 0x49, 0xD7, 0xE8, 0xBF, 0x86, 0xD0, 0xDC, 0xAE, 0x8C, 0x99, 0x28};

// Compute the parity of the bits under `opm` for plaintext `pt`.
uint8_t trailSbox(uint8_t pt, uint16_t opm)
{
    uint8_t *ptr = &ipm_masks[3 * opm];
    int a = P8(pt & *ptr);
    int b = P8(pt & *(ptr + 1));
    int c = P8(pt & *(ptr + 2));
    return (5 - 2 * (a + b + c + (a ^ b) + (a ^ c))) > 0;
}

void testIteration(uint32_t *key, uint16_t rounds, uint64_t sampleSize, uint8_t *keyGuess)
{
    // Variables
    uint32_t pt[4];
    uint8_t ipp, opp, part;
    uint64_t counter = sampleSize;

    uint8_t opm = 0x01;
    uint8_t opms[4] = {0x16, 0x3B, 0x2D, 0x2D};

    for (uint64_t cnt = 0; cnt < sampleSize; cnt++)
    {
        // Generate plaintext
        for (uint16_t j = 0; j < 4; j++)
        {
            pt[j] = rand_uint32();
        }

        // Analyse plaintext
        ipp = 0;
        for (uint16_t j = 0; j < 4; j++)
        {
            part = pt[j] >> (8 * (3-j)) & 0xFF;
            part ^= keyGuess[j];
            ipp ^= trailSbox(part, opms[j]);
        }

        // Encrypt
        // AddRoundKey(pt, key, 0);
        // SubBytes(pt);
        encrypt(pt, key, rounds);

        // Analyse cipher text.
        // opp = 0;
        // for (uint16_t j = 0; j < 4; j++)
        // {
        //     part = pt[j] >> (8 * (3-j)) & 0xFF;
        //     ipp ^= P8(part & opms[j]);
        // }
        opp = P8((pt[0] >> 24) & opm);

        // Update counters
        counter -= ipp ^ opp;
    }

    printResults(&counter, 1, sampleSize);
}


void monteCarloTest()
{
    // Settings
    uint64_t sampleSize = 0x010000000;
    uint16_t nrRandomTests = 16;
    uint16_t rounds = 2;

    // Key
    uint32_t key[4];
    for (uint16_t i = 0; i < 4; i++)
    {
        key[i] = rand_uint32();
    }
    uint32_t expandedKey[4 * (rounds + 1)];
    ExpandKey(key, expandedKey, rounds + 1);

    // Execute a number of tests with random key guesses.
    uint8_t keyGuess[4];
    uint32_t kg;
    for (uint16_t i = 0; i < nrRandomTests; i++)
    {
        // Set random key.
        kg = rand_uint32();
        for (uint16_t j = 0; j < 4; j++)
        {
            keyGuess[j] = kg >> (8 * (3-j)) & 0xFF;
        }

        // Execute test
        testIteration(key, rounds, sampleSize, keyGuess);
    }

    // Execute a test with the correct key guess
    for (uint16_t j = 0; j < 4; j++)
    {
        keyGuess[j] = key[j] >> (8 * (3-j)) & 0xFF;
    }
    testIteration(key, rounds, sampleSize, keyGuess);

}

int main()
{
    monteCarloTest();
}