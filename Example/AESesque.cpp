#include <iostream>
#include "AESesque.h"

// CYclic LEft SHift 32-bit word
#define cylesh32_4(word)  (word <<  4 ^ word >> 28)
#define cylesh32_8(word)  (word <<  8 ^ word >> 24)
#define cylesh32_12(word) (word << 12 ^ word >> 20)

// CYclic LEft SHift all 4-bit SEctions in 32-bit word.
#define secylesh32_1(word) ((word & 0x77777777) << 1 ^ (word & 0x88888888) >> 3)
#define secylesh32_2(word) ((word & 0x33333333) << 2 ^ (word & 0xCCCCCCCC) >> 2)

static const uint8_t S[] = {8, 2, 4, 0, 15, 5, 7, 12, 10, 6, 11, 3, 14, 13, 9, 1};

inline void SubBytes(uint32_t *word)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            // Apply Sbox to 4 LSBits and left cyclic cycle.
            // After 8 applications, the entire row is "Sboxed".
            word[i] = (word[i] & 0xFFFFFFF0) ^ S[word[i] & 0xF];
            word[i] = cylesh32_4(word[i]);
        }
    }
}

inline void ShiftRows(uint32_t *word)
{
    word[1] = cylesh32_4(word[1]);
    word[2] = cylesh32_8(word[2]);
    word[3] = cylesh32_12(word[3]);
}

inline void MixColumns(uint32_t *word)
{
    uint32_t temp[4];
    uint32_t val1, val2;

    // Compute mixing
    for (uint8_t i = 0; i < 4; i++)
    {
        val1 = word[(i + 2) % 4];
        val2 = word[(i + 3) % 4];
        temp[i] = word[(i + 1) % 4] ^ secylesh32_1(val1) ^ secylesh32_2(val2);
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        word[i] = temp[i];
    }
}

inline void Add(uint32_t *word, uint32_t *value)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        word[i] ^= value[i];
    }
}

void encrypt(uint32_t *word, uint32_t *key, uint32_t * constants, uint8_t rounds)
{
    // Add key
    // Add(word, key);

    for (uint8_t i = 0; i < rounds; i++)
    {
        SubBytes(word);
        ShiftRows(word);
        MixColumns(word);
        Add(word, constants);
        Add(word, key);
    }

    // Add permuted key
    // uint32_t perm_key[4];
    // perm_key[0] = (key[0] >> 1) ^ (key[3] << 31);
    // perm_key[1] = (key[1] >> 1) ^ (key[0] << 31) ^ (key[0] >> 31);
    // perm_key[2] = (key[2] >> 1) ^ (key[1] << 31) ^ (key[1] >> 31) ^ (key[0] << 1);
    // perm_key[3] = (key[3] >> 1) ^ (key[2] << 31) ^ (key[2] >> 31) ^ (key[1] << 1);
    // Add(word, perm_key);
}
