#include "aes.h"
#include <iostream>
#include "toolbox.h"

void encrypt(uint32_t * State, uint32_t * ExpandedKey, uint32_t nr_rounds)
{
    // Add initial round key
    AddRoundKey(State, ExpandedKey, 0);
    for (uint8_t i = 1; i < nr_rounds; i++)
    {
        SubBytes(State);
        ShiftRows(State);
        MixColumns(State);
        AddRoundKey(State, ExpandedKey, i);
    }
    SubBytes(State);
    ShiftRows(State);
    AddRoundKey(State, ExpandedKey, nr_rounds);
}