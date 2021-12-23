#include "encrypt.h"
#include <iostream>

void print_name(){
    std::cout << "EncryptV2" << '\n';
}

// CYclic LEft SHift
uint16_t cylesh_1(uint16_t word)
{
    return (word << 1) ^ (word >> 15);
}

uint16_t cylesh_2(uint16_t word)
{
    return (word << 2) ^ (word >> 14);
}

uint16_t cylesh_8(uint16_t word)
{
    return (word << 8) ^ (word >> 8);
}

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