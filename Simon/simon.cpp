#include <iostream>
#include "simon.h"

// CYclic LEft SHift
#define cylesh_1(word) (word << 1 ^ word >> 15)
#define cylesh_2(word) (word << 2 ^ word >> 14)
#define cylesh_8(word) (word << 8 ^ word >> 8)

uint32_t encrypt(uint32_t word, uint16_t *subkeys, uint8_t rounds)
{
    // Encrypt word
    uint16_t tmp, left = word >> 16, right = word & 0xFFFF;
    for (uint8_t i = 0; i < rounds; i++)
    {
        tmp = left;
        left = right ^ (cylesh_1(left) & cylesh_8(left)) ^ cylesh_2(left) ^ subkeys[i];
        right = tmp;
    }
    return (((uint32_t)left) << 16) ^ right;
}

uint32_t decrypt(uint32_t word, uint16_t *subkeys, uint8_t rounds)
{
    // Decrypt word
    uint16_t tmp, left = word >> 16, right = word & 0xFFFF;
    for (uint8_t i = rounds; i > 0; i--)
    {
        tmp = right;
        right = left ^ (cylesh_1(right) & cylesh_8(right)) ^ cylesh_2(right) ^ subkeys[i - 1];
        left = tmp;
    }
    return (((uint32_t)left) << 16) ^ right;
}
