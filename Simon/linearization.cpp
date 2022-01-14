#include <iostream>
#include <stdio.h>
#include "simon.h"
#include "toolbox.h"
#include <bitset>

static uint32_t ct;
static const uint64_t KEY = 0x48378AF8BB87914A;
static const uint32_t NR_ITERATIONS = 0x40000;

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
    uint32_t xl, xr;
    uint16_t X1R1xX1R10, X1R6xX1R13;

    uint16_t k0 = subkeys[0];
    for (uint32_t i = 0; i < NR_ITERATIONS; i++)
    {
        pt = rand_uint32();
        in_parity = getParity(pt & ipm);

        // Encryption
        ct = encrypt(pt, subkeys, rounds);
        xl = ct >> 16;
        xr = ct & 0xFFFF;

        // Linear approximation
        out_parity = F(xl, 2) ^ F(k0, 2) ^ F(xl, 14) ^ F(k0, 14) ^ F(xr, 12) ^ L((
              I(0)
            + I(F(xr,  1))
            + I(F(xr, 10))
            - I(F(xr,  1) ^ F(xr, 10))
            + I(F(xr,  6))
            + I(F(xr,  6) ^ F(xr,  1))
            + I(F(xr,  6) ^ F(xr, 10))
            - I(F(xr,  6) ^ F(xr,  1) ^ F(xr, 10))
            + I(F(xr, 13))
            + I(F(xr, 13) ^ F(xr, 1))
            + I(F(xr, 13) ^ F(xr, 10))
            - I(F(xr, 13) ^ F(xr,  1) ^ F(xr, 10))
            - I(F(xr,  6) ^ F(xr, 13))
            - I(F(xr,  6) ^ F(xr, 13) ^ F(xr,  1))
            - I(F(xr,  6) ^ F(xr, 13) ^ F(xr, 10))
            + I(F(xr,  6) ^ F(xr, 13) ^ F(xr,  1) ^ F(xr, 10))
        ) >> 2);

        count += out_parity ^ in_parity;
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
    uint16_t xl, xr, X0R2, X0R14;
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

        X0R2 = F(k1, 14) ^ F(k2, 12) ^ F(xl, 12) ^ F(k1, 2) ^ F(xr, 2) ^ L((
             (I(0) << 1)
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
        X0R14 = F(xr, 10) ^ F(xr, 14) ^ L((
             (I(0) << 1)
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
            +(I(F(xr, 4)) << 1)
            + I(F(xr, 4) ^ F(xl, 13) ^ F(k2, 13) ^ F(xr, 11))
            + I(F(xr, 4) ^ F(xl, 13) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 5))
            + I(F(xr, 4) ^ F(xl, 13) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 12))
            - I(F(xr, 4) ^ F(xl, 13) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 5) ^ F(xr, 12))
            + I(F(xl, 6) ^ F(k2, 6))
            + I(F(xl, 6) ^ F(k2, 6) ^ F(xr, 5))
            + I(F(xl, 6) ^ F(k2, 6) ^ F(xr, 14))
            - I(F(xl, 6) ^ F(k2, 6) ^ F(xr, 5) ^ F(xr, 14))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 11))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 5))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 12) ^ F(xr, 14))
            + I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 11) ^ F(xr, 5) ^ F(xr, 12) ^ F(xr, 14))
            +(I(F(xr, 11)) << 1)
            + I(F(xl, 13) ^ F(k2, 13))
            + I(F(xl, 13) ^ F(k2, 13) ^ F(xr, 5))
            + I(F(xl, 13) ^ F(k2, 13) ^ F(xr, 12))
            - I(F(xl, 13) ^ F(k2, 13) ^ F(xr, 5) ^ F(xr, 12))
            + I(F(xr, 11) ^ F(xl, 6) ^ F(k2, 6) ^ F(xr, 4))
            + I(F(xr, 11) ^ F(xl, 6) ^ F(k2, 6) ^ F(xr, 4) ^ F(xr, 5))
            + I(F(xr, 11) ^ F(xl, 6) ^ F(k2, 6) ^ F(xr, 4) ^ F(xr, 14))
            - I(F(xr, 11) ^ F(xl, 6) ^ F(k2, 6) ^ F(xr, 4) ^ F(xr, 5) ^ F(xr, 14))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 4))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 4) ^ F(xr, 5))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 4) ^ F(xr, 12) ^ F(xr, 14))
            + I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 4) ^ F(xr, 5) ^ F(xr, 12) ^ F(xr, 14))
            -(I(F(xr, 4) ^ F(xr, 11)) << 1)
            - I(F(xr, 4) ^ F(xl, 13) ^ F(k2, 13))
            - I(F(xr, 4) ^ F(xl, 13) ^ F(k2, 13) ^ F(xr, 5))
            - I(F(xr, 4) ^ F(xl, 13) ^ F(k2, 13) ^ F(xr, 12))
            + I(F(xr, 4) ^ F(xl, 13) ^ F(k2, 13) ^ F(xr, 5) ^ F(xr, 12))
            - I(F(xr, 11) ^ F(xl, 6) ^ F(k2, 6))
            - I(F(xr, 11) ^ F(xl, 6) ^ F(k2, 6) ^ F(xr, 5))
            - I(F(xr, 11) ^ F(xl, 6) ^ F(k2, 6) ^ F(xr, 14))
            + I(F(xr, 11) ^ F(xl, 6) ^ F(k2, 6) ^ F(xr, 5) ^ F(xr, 14))
            + I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13))
            + I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 5))
            + I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 12) ^ F(xr, 14))
            - I(F(xl, 6) ^ F(xl, 13) ^ F(k2, 6) ^ F(k2, 13) ^ F(xr, 5) ^ F(xr, 12) ^ F(xr, 14))
        ) >> 3);

        count += in_parity ^ X0R2 ^ X0R14;
    }
    return count;
}

uint32_t linearizeParity3()
{
    const uint8_t rounds = 3;
    const uint32_t ipm = 0x40040001;
    // const uint32_t opm = 0x00040000;

    // Compute key
    uint16_t subkeys[rounds];
    generateSubKeys(KEY, subkeys, rounds);

    uint32_t count = 0;
    uint32_t pt, in_parity;
    uint16_t xl, xr;
    uint16_t k1 = subkeys[0], k2 = subkeys[1], k3 = subkeys[2];

    for (uint32_t i = 0; i < NR_ITERATIONS; i++)
    {
        pt = rand_uint32();

        // Encryption
        in_parity = getParity(pt & ipm);

        // Linear approximation
        ct = encrypt(pt, subkeys, rounds);
        xl = ct >> 16;
        xr = ct & 0xFFFF;

        uint16_t X2R1xX2R10 =  L((
              (I(0) << 1)
            + I(F(xl,  1) ^ F(k3,  1) ^ F(xr, 15))
            + I(F(xl,  1) ^ F(k3,  1) ^ F(xr, 15) ^ F(xr,  0))
            + I(F(xl,  1) ^ F(k3,  1) ^ F(xr, 15) ^ F(xr,  9))
            - I(F(xl,  1) ^ F(k3,  1) ^ F(xr, 15) ^ F(xr,  0) ^ F(xr,  9))
            + I(F(xl, 10) ^ F(k3, 10) ^ F(xr,  8))
            + I(F(xl, 10) ^ F(k3, 10) ^ F(xr,  8) ^ F(xr,  9))
            + I(F(xl, 10) ^ F(k3, 10) ^ F(xr,  8) ^ F(xr,  2))
            - I(F(xl, 10) ^ F(k3, 10) ^ F(xr,  8) ^ F(xr,  9) ^ F(xr,  2))
            - I(F(xl,  1) ^ F(k3,  1) ^ F(xr, 15) ^ F(xl, 10) ^ F(k3, 10) ^ F(xr,  8))
            - I(F(xl,  1) ^ F(k3,  1) ^ F(xr, 15) ^ F(xl, 10) ^ F(k3, 10) ^ F(xr,  8) ^ F(xr,  0) ^ F(xr,  2))
            - I(F(xl,  1) ^ F(k3,  1) ^ F(xr, 15) ^ F(xl, 10) ^ F(k3, 10) ^ F(xr,  8) ^ F(xr,  9))
            + I(F(xl,  1) ^ F(k3,  1) ^ F(xr, 15) ^ F(xl, 10) ^ F(k3, 10) ^ F(xr,  8) ^ F(xr,  0) ^ F(xr,  9) ^ F(xr,  2))
        ) >> 2);

        uint16_t X1R8 = F(xl, 6) ^ F(k3, 6) ^ F(xr, 4) ^ F(xr, 8) ^ F(k2, 8) ^ L((
             (I(0) << 1)
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xr, 15))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xr,  8))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xr, 15) ^ F(xr,  8))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  5))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 15))
            - I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6) ^ F(xr, 15))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 15))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  8) ^ F(xr,  6))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 15) ^ F(xr,  8) ^ F(xr,  6))
            +(I(F(xr,  5)) << 1)
            + I(F(xr,  5) ^ F(xl,  0) ^ F(k3,  0) ^ F(xr, 14))
            + I(F(xr,  5) ^ F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xr, 15))
            + I(F(xr,  5) ^ F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xr,  8))
            - I(F(xr,  5) ^ F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xr, 15) ^ F(xr,  8))
            + I(F(xl,  7) ^ F(k3,  7))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  6))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr, 15))
            - I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  6) ^ F(xr, 15))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xl,  7) ^ F(k3,  7))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 15))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  8) ^ F(xr,  6))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 15) ^ F(xr,  8) ^ F(xr,  6))
            +(I(F(xr, 14)) << 1)
            + I(F(xl,  0) ^ F(k3,  0))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 15))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xr,  8))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xr, 15) ^ F(xr,  8))
            + I(F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5))
            + I(F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6))
            + I(F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 15))
            - I(F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6) ^ F(xr, 15))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 15))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  8) ^ F(xr,  6))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 15) ^ F(xr,  8) ^ F(xr,  6))
            -(I(F(xr,  5) ^ F(xr, 14)) << 1)
            - I(F(xr,  5) ^ F(xl,  0) ^ F(k3,  0))
            - I(F(xr,  5) ^ F(xl,  0) ^ F(k3,  0) ^ F(xr, 15))
            - I(F(xr,  5) ^ F(xl,  0) ^ F(k3,  0) ^ F(xr,  8))
            + I(F(xr,  5) ^ F(xl,  0) ^ F(k3,  0) ^ F(xr, 15) ^ F(xr,  8))
            - I(F(xr, 14) ^ F(xl,  7) ^ F(k3,  7))
            - I(F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  6))
            - I(F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 15))
            + I(F(xr, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  6) ^ F(xr, 15))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xl,  7) ^ F(k3,  7))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 15))
            + I(F(xl,  0) ^ F(k3,  0) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  8) ^ F(xr,  6))
            - I(F(xl,  0) ^ F(k3,  0) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 15) ^ F(xr,  8) ^ F(xr,  6))
        ) >> 3);

        uint16_t X1R15 = F(xl, 13) ^ F(k3, 13) ^ F(xr, 11) ^ F(xr, 15) ^ F(k2, 15) ^ L((
             (I(0) << 1)
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xr, 13))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xr,  6))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xr, 13) ^ F(xr,  6))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  5))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 15))
            - I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6) ^ F(xr, 15))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 13) ^ F(xr, 15))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 13) ^ F(xr,  6) ^ F(xr, 15))
            +(I(F(xr, 12)) << 1)
            + I(F(xl, 14) ^ F(k3, 14))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 13))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xr,  6))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 13) ^ F(xr,  6))
            + I(F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5))
            + I(F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6))
            + I(F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 15))
            - I(F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6) ^ F(xr, 15))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 13) ^ F(xr, 15))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr,  6))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  5) ^ F(xr, 13) ^ F(xr,  6) ^ F(xr, 15))
            +(I(F(xr,  5)) << 1)
            + I(F(xr,  5) ^ F(xl, 14) ^ F(k3, 14) ^ F(xr, 12))
            + I(F(xr,  5) ^ F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xr, 13))
            + I(F(xr,  5) ^ F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xr,  6))
            - I(F(xr,  5) ^ F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xr, 13) ^ F(xr,  6))
            + I(F(xl,  7) ^ F(k3,  7))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  6))
            + I(F(xl,  7) ^ F(k3,  7) ^ F(xr, 15))
            - I(F(xl,  7) ^ F(k3,  7) ^ F(xr,  6) ^ F(xr, 15))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xl,  7) ^ F(k3,  7))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 13) ^ F(xr, 15))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  6))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 13) ^ F(xr,  6) ^ F(xr, 15))
            -(I(F(xr, 12) ^ F(xr,  5)) << 1)
            - I(F(xr,  5) ^ F(xl, 14) ^ F(k3, 14))
            - I(F(xr,  5) ^ F(xl, 14) ^ F(k3, 14) ^ F(xr, 13))
            - I(F(xr,  5) ^ F(xl, 14) ^ F(k3, 14) ^ F(xr,  6))
            + I(F(xr,  5) ^ F(xl, 14) ^ F(k3, 14) ^ F(xr, 13) ^ F(xr,  6))
            - I(F(xr, 12) ^ F(xl,  7) ^ F(k3,  7))
            - I(F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  6))
            - I(F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 15))
            + I(F(xr, 12) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  6) ^ F(xr, 15))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xl,  7) ^ F(k3,  7))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 13) ^ F(xr, 15))
            + I(F(xl, 14) ^ F(k3, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr,  6))
            - I(F(xl, 14) ^ F(k3, 14) ^ F(xl,  7) ^ F(k3,  7) ^ F(xr, 13) ^ F(xr,  6) ^ F(xr, 15))
        ) >> 3);;

        uint16_t X1R8xX1R15 = X1R8 & X1R15;
        uint16_t out_parity = F(xr, 2) ^ F(k2, 2) ^ F(k1, 0) ^ X2R1xX2R10 ^ X1R8xX1R15;

        count += in_parity ^ out_parity;
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

void printResult(uint32_t count)
{
    if (count)
    {
        std::cout << "INCORRECT: " << count << std::endl;
    }
    else
    {
        std::cout << "CORRECT" << std::endl;
    }
}

int main()
{
    uint32_t count = linearizeParity1();
    printResult(count);
    count = linearizeParity2();
    printResult(count);
    count = linearizeParity3();
    printResult(count);
    return 0;
}
