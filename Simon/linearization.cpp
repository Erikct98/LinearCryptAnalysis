#include <iostream>
#include <stdio.h>
#include "simon.h"
#include "toolbox.h"
#include <bitset>

static uint32_t ct;
static const uint64_t KEY = 0x48378AF8BB87914A;
static const uint32_t NR_ITERATIONS = 0x400;

// #define AND(a, b) 0.5 * ((-1)^0 + (-1)^a + (-1)^b + (-1)^(a+b))
#define AND(a, b) ~((~(a << 1) + ~(b << 1) + ((a ^ b) << 1)) >> 2)

// Compute non-linear operations (AND) as sum of linear operations (XOR)
#define lin(a) (1 - (a << 1))
#define EXP1(a, x) lin((a >> x & 0x1))
#define EXP2(a, x, y) lin(((a >> x ^ a >> y) & 0x1))
#define EXP3(a, x, y, z) lin(((a >> x ^ a >> y ^ a >> z) & 0x1))
#define EXP4(a, w, x, y, z) lin(((a >> w ^ a >> x ^ a >> y ^ a >> z) & 0x1))

// Performs the {0, 1} -> {1, -1} mapping.
// I.e., computes (-1)^a
int I(uint16_t a)
{
    return 1 - (a << 1);
}

// Performs the {1, -1} -> {0, 1} mapping.
// I.e. computes log(a) / log(-1)
uint16_t L(int a)
{
    return (1 - a) >> 1;
}

#define F(a, b) (a >> b & 0x1)

uint32_t linearizeParity1()
{
    const uint8_t rounds = 1;
    const uint32_t ipm = 0x00014004;
    const uint32_t opm = 0x40041000;

    // Compute key
    uint16_t subkeys[rounds];
    generateSubKeys(KEY, subkeys, rounds);

    uint32_t count = 0;
    uint32_t pt, ct, in_parity, out_parity;
    int32_t nlp, res;
    int32_t A, B, C, D;
    for (uint32_t i = 0; i < NR_ITERATIONS; i++)
    {
        pt = rand_uint32();

        // Encryption
        in_parity = getParity(pt & ipm);

        // Linear approximation
        ct = encrypt(pt, subkeys, rounds);
        out_parity = getParity(ct & opm);
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

uint32_t linearizeParity2()
{
    const uint8_t rounds = 2;
    const uint32_t ipm = 0x00014004;
    const uint32_t opm = 0x10004404;

    // Compute key
    uint16_t subkeys[rounds];
    generateSubKeys(KEY, subkeys, rounds);

    uint32_t count = 0;
    uint32_t pt, in_parity;
    uint16_t xl, xr, X1R12, X1R1xX1R10, X1R6xX1R13, X0R2plusX2L0, X0R14;
    uint16_t k1 = subkeys[0], k2 = subkeys[1];
    for (uint32_t i = 0; i < NR_ITERATIONS; i++)
    {
        pt = rand_uint32();

        // Encryption
        in_parity = getParity(pt & ipm);

        // Linear approximation
        ct = encrypt(pt, subkeys, rounds);

        xl = ct >> 16;
        xr = ct & 0xFFFF;

        X1R12 = F(xl, 12) ^ F(k2, 12) ^ F(xr, 10) ^ L((I(0) + I(F(xr, 4)) + I(F(xr, 11)) - I(F(xr, 4) ^ F(xr, 11))) >> 1);

        X1R1xX1R10 = L((
            (I(0) << 2)
            + I(F(xl, 1) ^ F(k2, 1) ^ F(xr, 15))
            + I(F(xl, 1) ^ F(k2, 1) ^ F(xr, 15) ^ F(xr, 0))
            + I(F(xl, 1) ^ F(k2, 1) ^ F(xr, 15) ^ F(xr, 9))
            - I(F(xl, 1) ^ F(k2, 1) ^ F(xr, 15) ^ F(xr, 0) ^ F(xr, 9))
            + I(F(xl, 10) ^ F(k2, 10) ^ F(xr, 8))
            + I(F(xl, 10) ^ F(k2, 10) ^ F(xr, 8) ^ F(xr, 2))
            + I(F(xl, 10) ^ F(k2, 10) ^ F(xr, 8) ^ F(xr, 9))
            - I(F(xl, 10) ^ F(k2, 10) ^ F(xr, 8) ^ F(xr, 2) ^ F(xr, 9))
            - I(F(xl, 1) ^ F(xl, 10) ^ F(k2, 1) ^ F(k2, 10) ^ F(xr, 8) ^ F(xr, 15))
            - I(F(xl, 1) ^ F(xl, 10) ^ F(k2, 1) ^ F(k2, 10) ^ F(xr, 8) ^ F(xr, 15) ^ F(xr, 9))
            - I(F(xl, 1) ^ F(xl, 10) ^ F(k2, 1) ^ F(k2, 10) ^ F(xr, 8) ^ F(xr, 15) ^ F(xr, 0) ^ F(xr, 2))
            + I(F(xl, 1) ^ F(xl, 10) ^ F(k2, 1) ^ F(k2, 10) ^ F(xr, 8) ^ F(xr, 15) ^ F(xr, 0) ^ F(xr, 2) ^ F(xr, 9))
        ) >> 2);
        X1R6xX1R13 = L((
            I(0)
            + I(F(xl, 13) ^ F(k2, 13) ^ F(xr, 11))
            + I(F(xl, 13) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 5))
            + I(F(xl, 13) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 12))
            - I(F(xl, 13) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 5) ^ F(xr, 12))
            + I(F(xl, 6) ^ F(k2, 6) ^ F(xr, 4))
            + I(F(xl, 6) ^ F(k2, 6) ^ F(xr, 4) ^ F(xr, 5))
            + I(F(xl, 6) ^ F(k2, 6) ^ F(xr, 4) ^ F(xr, 14))
            - I(F(xl, 6) ^ F(k2, 6) ^ F(xr, 4) ^ F(xr, 5) ^ F(xr, 14))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 4) ^ F(xr, 11))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 4) ^ F(xr, 11) ^ F(xr, 5))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 4) ^ F(xr, 11) ^ F(xr, 12) ^ F(xr, 14))
            + I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 4) ^ F(xr, 11) ^ F(xr, 5) ^ F(xr, 12) ^ F(xr, 14))
        ) >> 2);

        X0R2plusX2L0 = F(xr, 2) ^ F(k1, 2) ^ X1R1xX1R10;
        X0R14 = F(xr, 14) ^ F(k1, 14) ^ X1R12 ^ X1R6xX1R13;

        count += in_parity ^ X0R2plusX2L0 ^ X0R14;
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
    uint32_t count = linearizeParity2();
    if (count)
    {
        std::cout << "INCORRECT: " << count << std::endl;
    }
    else
    {
        std::cout << "CORRECT" << std::endl;
    }
    return 0;
}
