#include "encrypt.h"
#include <iostream>

void print_name(){
    std::cout << "EncryptV1" << '\n';
}

// CYclic LEft SHift
uint16_t cylesh(uint16_t word, uint8_t ticks)
{
    return (word << ticks) ^ (word >> (16 - ticks));
}

uint32_t encrypt(uint32_t word, uint16_t *subkeys, uint8_t rounds)
{
    // Encrypt word
    uint16_t tmp, left = word >> 16, right = word & 0xFFFF;
    for (uint8_t i = 0; i < rounds; i++)
    {
        tmp = left;
        left = right ^ (cylesh(left, 1) & cylesh(left, 8)) ^ cylesh(left, 2) ^ subkeys[i];
        right = tmp;
    }
    return (((uint32_t)left) << 16) ^ right;
}

