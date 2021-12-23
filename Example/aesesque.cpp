#include <iostream>
#include "aesesque.h"

uint8_t S[] = {8, 2, 4, 0, 15, 5, 7, 12, 10, 6, 11, 3, 14, 13, 9, 1};

uint64_t generateRandom64Bits()
{
    uint64_t word = rand();
    word = (word << 31) ^ rand();
    word = (word << 31) ^ rand();
    return word;
}

// Found on https://graphics.stanford.edu/~seander/bithacks.html
uint64_t getParity(uint64_t v)
{
    v ^= v >> 32;
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v &= 0xf;
    return (0x6996 >> v) & 1;
}

uint8_t cylesh(uint8_t word, uint8_t ticks)
{
    return ((word << ticks) ^ (word >> 4 - ticks)) & 0xF;
}

void subBytes(uint8_t *grid)
{
    for (uint8_t i = 0; i < 32; i++)
    {
        grid[i] = S[grid[i]];
    }
}

void shiftRows(uint8_t *grid)
{
    uint8_t tmp1, tmp2, tmp3;

    // Shift over the second row
    tmp1 = grid[1];
    for (int i = 1; i < 28; i += 4)
    {
        grid[i] = grid[i + 4];
    }
    grid[29] = tmp1;

    // Shift over the third row
    tmp1 = grid[2], tmp2 = grid[6];
    for (int i = 2; i < 24; i += 4)
    {
        grid[i] = grid[i + 8];
    }
    grid[26] = tmp1, grid[30] = tmp2;

    // Shift over the fourth row
    tmp1 = grid[3], tmp2 = grid[7], tmp3 = grid[11];
    for (int i = 3; i < 20; i += 4)
    {
        grid[i] = grid[i + 12];
    }
    grid[23] = tmp1, grid[27] = tmp2, grid[31] = tmp3;
}

void mixColumns(uint8_t *grid)
{
    uint8_t tmp[4];
    for (uint8_t i = 0; i < 8; i++)
    {
        // Make copy of values
        for (uint8_t j = 0; j < 4; j++)
        {
            tmp[j] = grid[4 * i + j];
        }

        // Compute new column values
        for (uint8_t j = 0; j < 4; j++)
        {
            grid[4 * i + j] = tmp[(j + 1) % 4] ^ cylesh(tmp[(j + 2) % 4], 1) ^ cylesh(tmp[(j + 3) % 4], 2);
        }
    }
}

void make_grid(uint64_t *word, uint8_t *grid)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            grid[16 * i + j] = (word[i] >> (4 * j)) & 0xF;
        }
    }
}

void degridify(uint8_t *grid, uint64_t *res)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        res[i] = 0;
        for (int8_t j = 15; j >= 0; j--)
        {
            res[i] <<= 4;
            res[i] ^= grid[16 * i + j];
        }
    }
}

void encrypt(uint64_t *word, uint64_t *res)
{
    // Turn it into byteword
    uint8_t grid[32];
    make_grid(word, grid);
    for (uint8_t i = 0; i < 8; i++)
    {
        subBytes(grid);
        shiftRows(grid);
        mixColumns(grid);
    }
    degridify(grid, res);
}
