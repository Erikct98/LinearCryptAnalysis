#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include "simon.h"
#include "toolbox.h"

int main()
{
    uint8_t rounds = 32;
    uint64_t key = 0x1918111009080100;

    uint16_t subkeys[rounds];
    generateSubKeys(key, subkeys, rounds);

    uint32_t pt = 0x65656877;
    uint32_t exp_ct = 0xc69be9bb;

    uint32_t ct = encrypt(pt, subkeys, rounds);
    std::cout << exp_ct - ct << '\n';

    uint32_t dt = decrypt(ct, subkeys, rounds);
    std::cout << dt - pt << '\n';

    return 0;
}