#include <iostream>
#include <stdio.h>
#include "simon.h"
#include "toolbox.h"
#include <bitset>

static const uint64_t KEY = 0x48378AF8BB87914A;
static const uint8_t ROUNDS = 1;
static const uint32_t NR_ITERATIONS = 0x400;

static const uint32_t IPM = 0x00014004;
static const uint32_t OPM = 0x40041000;

// #define AND(a, b) 0.5 * ((-1)^0 + (-1)^a + (-1)^b + (-1)^(a+b))
#define AND(a, b) ~((~(a << 1) + ~(b << 1) + ((a ^ b) << 1)) >> 2)

// Compute non-linear operations (AND) as sum of linear operations (XOR)
#define lin(a) (1 - (a << 1))
#define EXP1(a, x) lin((a >> x & 0x1))
#define EXP2(a, x, y) lin(((a >> x ^ a >> y) & 0x1))
#define EXP3(a, x, y, z) lin(((a >> x ^ a >> y ^ a >> z) & 0x1))
#define EXP4(a, w, x, y, z) lin(((a >> w ^ a >> x ^ a >> y ^ a >> z) & 0x1))

uint32_t linearizeParity()
{
    // Compute key
    uint16_t subkeys[ROUNDS];
    generateSubKeys(KEY, subkeys, ROUNDS);

    uint32_t count = 0;
    uint32_t pt, ct, in_parity, out_parity;
    int32_t nlp, res;
    int32_t A, B, C, D;
    for (uint32_t i = 0; i < NR_ITERATIONS; i++)
    {
        pt = rand_uint32();

        // Encryption
        in_parity = getParity(pt & IPM);

        // Linear approximation
        ct = encrypt(pt, subkeys, ROUNDS);
        out_parity = getParity(ct & OPM);
        A = 1 + EXP1(ct, 1) + EXP1(ct, 10) - EXP2(ct, 1, 10);
        B = EXP1(ct, 6) + EXP2(ct, 1, 6) + EXP2(ct, 6, 10) - EXP3(ct, 1, 6, 10);
        C = EXP1(ct, 13) + EXP2(ct, 1, 13) + EXP2(ct, 10, 13) - EXP3(ct, 1, 10, 13);
        D = -EXP2(ct, 6, 13) - EXP3(ct, 1, 6, 13) - EXP3(ct, 6, 10, 13) + EXP4(ct, 1, 6, 10, 13);
        count += out_parity ^ in_parity ^ (1 - ((A + B + C + D) >> 2)) >> 1 ^ getParity(KEY & 0x4004);

        // Validation
        // std::cout << "in: " << in_parity << std::endl;
        // std::bitset<16> x(ct & 0xFFFF);
        // std::cout << "ct: " << x << std::endl;
        // std::cout << (int)EXP1(ct, 1) << (int)EXP1(ct, 10) << (int)EXP2(ct, 1, 10) << std::endl;
        // std::cout << "A: " << (int) (1 + EXP1(ct, 1) + EXP1(ct, 10) - EXP2(ct, 1, 10)) << std::endl;
        // std::cout << "B: " << (int) (EXP1(ct, 6) + EXP2(ct, 1, 6) + EXP2(ct, 6, 10) - EXP3(ct, 1, 6, 10)) << std::endl;
        // std::cout << "C: " << (int) (EXP1(ct, 13) + EXP2(ct, 1, 13) + EXP2(ct, 10, 13) - EXP3(ct, 1, 10, 13)) << std::endl;
        // std::cout << "D: " << (int) (-EXP2(ct, 6, 13) - EXP3(ct, 1, 6, 13) - EXP3(ct, 6, 10, 13) + EXP4(ct, 1, 6, 10, 13)) << std::endl;
        // std::cout << "out: " << out_parity << ", nlp: " << nlp << ", res: " << res << std::endl;
    }
    return count;
}

void test()
{
    uint8_t a, b;
    for (uint8_t i = 0; i < 4; i++)
    {
        a = i >> 1;
        b = i & 1;
        std::cout << a << "x" << b << "=" << AND(a, b) << std::endl;
    }
}

int main()
{
    uint32_t count = linearizeParity();
    if (count)
    {
        std::cout << "INCORRECT" << std::endl;
    }
    else
    {
        std::cout << "CORRECT" << std::endl;
    }
    return 0;
}