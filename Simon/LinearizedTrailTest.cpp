#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include "simon.h"
#include "toolbox.h"

static const uint32_t IPM = 0x40040001;
static const uint32_t ENC_ROUNDS = 7;
static const uint64_t SAMPLE_SIZE = 0x800000;
static const uint32_t NR_TESTS = 0x100;
static const uint16_t USE_KEYS = 0;

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

uint64_t compute_correlation_linearized(uint64_t key)
{
    // Constants
    uint64_t count = 0;
    uint32_t pt, ct, X0R2, X0R14, xl, xr;
    uint16_t subkeys[ENC_ROUNDS];
    uint16_t k1, k2;
    generateSubKeys(key, subkeys, ENC_ROUNDS);
    for (uint32_t i = 0; i < SAMPLE_SIZE; i++)
    {
        pt = rand_uint32();
        ct = encrypt(pt, subkeys, ENC_ROUNDS);

        // Approximate count
        xl = ct >> 16;
        xr = ct & 0xFFFF;

        k1 = subkeys[ENC_ROUNDS - 2] * USE_KEYS;
        k2 = subkeys[ENC_ROUNDS - 1] * USE_KEYS;

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

        count += P32(pt & IPM) ^ X0R2 ^ X0R14;
    }
    return count;
}

uint64_t compute_correlation_non_linearized(uint64_t key)
{
    // Constants
    uint32_t OPM = 0x10004404;
    uint64_t count = 0;
    uint32_t pt, ct, X0R2, X0R14, xl, xr;
    uint16_t subkeys[ENC_ROUNDS];
    uint16_t k7;
    uint16_t X6R12, X6R01, X6R10, X6R13, X6R06;
    generateSubKeys(key, subkeys, ENC_ROUNDS);
    for (uint32_t i = 0; i < SAMPLE_SIZE; i++)
    {
        pt = rand_uint32();
        ct = encrypt(pt, subkeys, ENC_ROUNDS);
        k7 = subkeys[ENC_ROUNDS - 1];
        X6R12 = F(ct, 11) & F(ct,  4); // ^ F(k7, 12);
        X6R01 = F(ct, 17) ^ F(k7,  1) ^ F(ct, 15) ^ F(ct,  0) & F(ct,  9);  // X7L[ 1] + k7[ 1] + X7R[15] + X7R[ 0] * X7R[ 9]
        X6R10 = F(ct, 26) ^ F(k7, 10) ^ F(ct,  8) ^ F(ct,  9) & F(ct,  2);  // X7L[10] + k7[10] + X7R[ 8] + X7R[ 9] * X7R[ 2]
        X6R13 = F(ct, 29) ^ F(k7, 13) ^ F(ct, 11) ^ F(ct, 12) & F(ct,  5);  // X7L[13] + k7[13] + X7R[11] + X7R[12] * X7R[ 5]
        X6R06 = F(ct, 22) ^ F(k7,  6) ^ F(ct,  4) ^ F(ct,  5) & F(ct, 14);  // X7L[ 6] + k7[ 6] + X7R[ 4] + X7R[ 5] * X7R[14]

        count += (P32(pt & IPM) ^ (X6L02 ^ X6R01 & X6R10 ^ X6L14 ^ X6R12 ^ X6R13 & X6R06)) & 1;
        count += P32(pt & IPM ^ ct & OPM) ^ X6R12 ^ X6R01 & X6R10 ^ X6R13 & X6R06;
    }
    return count;
}

// Dump results to file
void writeToFile(uint64_t *results, uint32_t nr_trials, std::string fname)
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

void execute()
{
    uint64_t results[NR_TESTS];
    uint64_t key;
    #pragma omp parallel for
    for (uint32_t i = 0; i < NR_TESTS; i ++)
    {
        key = rand_uint64();
        results[i] = compute_correlation_non_linearized(key);
    }
    std::string fname = generateFileName("results/LinearizedTrailTest", IPM, 0, SAMPLE_SIZE, NR_TESTS, ENC_ROUNDS);
    std::cout << fname << '\n';
    writeToFile(results, NR_TESTS, fname);
}

void speedtest()
{
    uint64_t results[NR_TESTS];
    uint64_t key;

    std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::_V2::system_clock::duration> ls, le, nls, nle;

    ls = std::chrono::high_resolution_clock::now();
    // for (uint32_t i = 0; i < NR_TESTS; i ++)
    // {
    //     key = rand_uint64();
    //     results[i] = compute_correlation_linearized(key);
    // }
    le = std::chrono::high_resolution_clock::now();

    nls = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < NR_TESTS; i ++)
    {
        key = rand_uint64();
        results[i] = compute_correlation_non_linearized(key);
    }
    nle = std::chrono::high_resolution_clock::now();

    // Print results to console
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(le - ls);
    std::cout << "L   duration (mus): " << duration.count() << '\n';
    duration = std::chrono::duration_cast<std::chrono::microseconds>(nle - nls);
    std::cout << "NL  duration (mus): " << duration.count() << '\n';
}

int main()
{
    std::srand(time(0));
    speedtest();
    // execute();

    return 0;
}