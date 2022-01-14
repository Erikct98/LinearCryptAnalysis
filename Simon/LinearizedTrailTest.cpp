#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include "simon.h"
#include "toolbox.h"

static const uint32_t IPM = 0x40040001;
static const uint32_t ENC_ROUNDS = 7;
static const uint32_t SAMPLE_SIZE = 0x100000;
static const uint32_t NR_TESTS = 0x10;

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

uint32_t compute_correlation(uint64_t key)
{
    // Constants
    uint32_t count = 0;
    uint32_t pt, ct, X0R2, X0R14, xl, xr;
    uint16_t subkeys[ENC_ROUNDS];
    generateSubKeys(key, subkeys, ENC_ROUNDS);
    for (uint32_t i = 0; i < SAMPLE_SIZE; i++)
    {
        pt = rand_uint32();
        ct = encrypt(pt, subkeys, ENC_ROUNDS);

        // Approximate count
        xl = ct >> 16;
        xr = ct & 0xFFFF;

        X0R2 = F(xl, 12) ^ F(xr, 2) ^ L((
             (I(0) << 1)
            + I(F(xl,  1) ^ F(xr, 15))
            + I(F(xl,  1) ^ F(xr, 15) ^ F(xr, 0))
            + I(F(xl,  1) ^ F(xr, 15) ^ F(xr, 9))
            - I(F(xl,  1) ^ F(xr, 15) ^ F(xr, 0) ^ F(xr, 9))
            + I(F(xl, 10) ^ F(xr,  8))
            + I(F(xl, 10) ^ F(xr,  8) ^ F(xr, 2))
            + I(F(xl, 10) ^ F(xr,  8) ^ F(xr, 9))
            - I(F(xl, 10) ^ F(xr,  8) ^ F(xr, 2) ^ F(xr, 9))
            - I(F(xl,  1) ^ F(xl, 10) ^ F(xr, 8) ^ F(xr, 15))
            - I(F(xl,  1) ^ F(xl, 10) ^ F(xr, 8) ^ F(xr, 15) ^ F(xr, 9))
            - I(F(xl,  1) ^ F(xl, 10) ^ F(xr, 8) ^ F(xr, 15) ^ F(xr, 0) ^ F(xr, 2))
            + I(F(xl,  1) ^ F(xl, 10) ^ F(xr, 8) ^ F(xr, 15) ^ F(xr, 0) ^ F(xr, 2) ^ F(xr, 9))
        ) >> 2);
        X0R14 = F(xr, 10) ^ F(xr, 14) ^ L((
             (I(0) << 1)
            + I(F(xl,  6))
            + I(F(xl,  6) ^ F(xl, 13))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  4))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  4) ^ F(xr,  5))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  4) ^ F(xr,  5) ^ F(xr, 11))
            + I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  4) ^ F(xr,  5) ^ F(xr, 12) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  4) ^ F(xr,  5) ^ F(xr, 11) ^ F(xr, 12) ^ F(xr, 14))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  4) ^ F(xr, 11))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  4) ^ F(xr, 11) ^ F(xr, 12) ^ F(xr, 14))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  4) ^ F(xr, 12) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  5))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr,  5) ^ F(xr, 12) ^ F(xr, 14))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr, 11))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr, 11) ^ F(xr, 5))
            - I(F(xl,  6) ^ F(xl, 13) ^ F(xr, 11) ^ F(xr, 12) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xl, 13) ^ F(xr, 11) ^ F(xr, 5) ^ F(xr, 12) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xl, 13) ^ F(xr, 12) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xr,  4))
            + I(F(xl,  6) ^ F(xr,  4) ^ F(xr, 5))
            - I(F(xl,  6) ^ F(xr,  4) ^ F(xr, 5) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xr,  4) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xr,  5))
            - I(F(xl,  6) ^ F(xr,  5) ^ F(xr, 14))
            - I(F(xl,  6) ^ F(xr, 11))
            + I(F(xl,  6) ^ F(xr, 11) ^ F(xr,  4))
            + I(F(xl,  6) ^ F(xr, 11) ^ F(xr,  4) ^ F(xr, 5))
            - I(F(xl,  6) ^ F(xr, 11) ^ F(xr,  4) ^ F(xr, 5) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xr, 11) ^ F(xr,  4) ^ F(xr, 14))
            - I(F(xl,  6) ^ F(xr, 11) ^ F(xr,  5))
            + I(F(xl,  6) ^ F(xr, 11) ^ F(xr,  5) ^ F(xr, 14))
            - I(F(xl,  6) ^ F(xr, 11) ^ F(xr, 14))
            + I(F(xl,  6) ^ F(xr, 14))
            + I(F(xl, 13))
            - I(F(xl, 13) ^ F(xr,  4))
            - I(F(xl, 13) ^ F(xr,  4) ^ F(xr,  5))
            + I(F(xl, 13) ^ F(xr,  4) ^ F(xr,  5) ^ F(xr, 11))
            - I(F(xl, 13) ^ F(xr,  4) ^ F(xr,  5) ^ F(xr, 11) ^ F(xr, 12))
            + I(F(xl, 13) ^ F(xr,  4) ^ F(xr,  5) ^ F(xr, 12))
            + I(F(xl, 13) ^ F(xr,  4) ^ F(xr, 11))
            + I(F(xl, 13) ^ F(xr,  4) ^ F(xr, 11) ^ F(xr, 12))
            - I(F(xl, 13) ^ F(xr,  4) ^ F(xr, 12))
            + I(F(xl, 13) ^ F(xr,  5))
            + I(F(xl, 13) ^ F(xr,  5) ^ F(xr, 11))
            - I(F(xl, 13) ^ F(xr,  5) ^ F(xr, 11) ^ F(xr, 12))
            - I(F(xl, 13) ^ F(xr,  5) ^ F(xr, 12))
            + I(F(xl, 13) ^ F(xr, 11))
            + I(F(xl, 13) ^ F(xr, 11) ^ F(xr, 12))
            + I(F(xl, 13) ^ F(xr, 12))
            +(I(F(xr,  4)) << 1)
            -(I(F(xr,  4) ^ F(xr, 11)) << 1)
            +(I(F(xr, 11)) << 1)
        ) >> 3);

        count += getParity(pt & IPM) ^ X0R2 ^ X0R14;
    }
    return count;
}

// Dump results to file
void writeToFile(uint32_t *results, uint32_t nr_trials, std::string fname)
{
    std::ofstream myfile(fname);
    if (myfile.is_open())
    {
        for (int i = 0; i < nr_trials; i++)
        {
            myfile << results[i] << '\n';
        }
        myfile.close();
    }
}

int main()
{
    std::srand(time(0));
    uint32_t results[NR_TESTS];
    uint64_t key;
    for (uint32_t i = 0; i < NR_TESTS; i ++)
    {
        key = rand_uint64();
        std::cout << key << '\n';
        results[i] = compute_correlation(key);
    }
    std::string fname = generateFileName("results/LinearizedTrailTest", IPM, 0, SAMPLE_SIZE, NR_TESTS, ENC_ROUNDS);
    std::cout << fname << '\n';
    writeToFile(results, NR_TESTS, fname);
    return 0;
}