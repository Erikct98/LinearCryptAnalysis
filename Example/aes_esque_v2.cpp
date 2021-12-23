#include <iostream>
#include "aesesque.h"

uint8_t S[] = {8, 2, 4, 0, 15, 5, 7, 12, 10, 6, 11, 3, 14, 13, 9, 1};
uint16_t cycle_masks[] = {0x1111, 0x3333, 0x7777};
uint16_t shift_masks[] = {0x0FFF, 0x00FF, 0x000F};

uint16_t sbox(uint16_t word)
{
    uint16_t res = 0, offset;
    for (uint8_t i = 0; i < 4; i++)
    {
        offset = i << 2;
        res ^= S[(word >> offset) & 0xF] << offset;
    }
    return res;
}

uint16_t cylesh_word(uint16_t word, uint8_t ticks)
{
    uint16_t mask = cycle_masks[ticks - 1];
    uint16_t neg = word & mask;
    word ^= neg;
    return (neg << ticks) ^ (word >> (4 - ticks));
}

uint16_t cylesh(uint16_t word, uint8_t ticks)
{
    return (word << ticks) ^ (word >> (16 - ticks));
}

void subBytes(uint16_t *grid)
{
    for (uint8_t i = 0; i < 8; i++)
        grid[i] = sbox(grid[i]);
}

void shiftRows(uint16_t *grid)
{
    uint16_t tmp;
    for (uint i = 0; i < 3; i++)
    {
        uint16_t mask = shift_masks[i];
        tmp = grid[0] & mask;
        for (uint16_t j = 0; j < 7; j++)
        {
            grid[i] ^= (grid[i] ^ grid[i + 1]) & mask;
        }
        grid[8] ^= (grid[8] ^ tmp) & mask;
    }
}

void mixColumns(uint16_t *grid)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        grid[i] = cylesh(grid[i], 4) ^ cylesh(cylesh_word(grid[i], 1), 8) ^ cylesh(cylesh_word(grid[i], 2), 12); // Something is off here.
    }
}

void make_grid(uint64_t *word, uint16_t *grid)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            grid[4 * i + j] = (uint16_t)(word[i] >> (j << 4));
        }
    }
}

void degridify(uint16_t *grid, uint64_t *res)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        res[i] = 0;
        for (uint8_t j = 0; j < 4; j++)
        {
            res[i] = (res[i] << 16) ^ grid[4 * i + j]; // Warning; inverse order
        }
    }
}

void encrypt(uint64_t *word)
{
    // Turn it into byteword
    uint16_t grid[8];
    make_grid(word, grid);
    for (uint8_t i = 0; i < 8; i++)
    {
        subBytes(grid);
        shiftRows(grid);
        mixColumns(grid);
    }
    uint64_t res[2];
    degridify(grid, res);
    std::cout << res[0] << res[1] << '\n';
}

int main()
{
    uint64_t word[] = {0xFEDCBA98FEDCBA98, 0xFEDCBA98FEDCBA98};
    encrypt(word);
    return 0;
}