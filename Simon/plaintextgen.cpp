#include <iostream>
#include <random>
#include <chrono>
#include <stdio.h>
#include "simon.h"
#include "toolbox.h"

uint64_t KEY = 0xfc692d85698c1806; // Some random key
uint8_t ENC_ROUNDS = 32;
uint64_t ITERATIONS = 0x80000000;
uint64_t SEED = 0;
uint32_t PRIME = 78367;
// uint32_t PRIME = 2971215073;

// Randomization
std::mt19937 _mt(0);
std::uniform_int_distribution<uint32_t> _randuint32(0, 0xFFFFFFFF);

inline uint32_t _rand_uint32()
{
    return _randuint32(_mt);
}

void testplaintextgen()
{
    _mt.seed(SEED);
    uint16_t subkeys[ENC_ROUNDS];
    generateSubKeys(KEY, subkeys, ENC_ROUNDS);

    std::chrono::_V2::system_clock::time_point start_a, stop_a, start_b, stop_b, start_c, stop_c, start_d, stop_d;
    uint32_t pt = 0;
    uint64_t checksum_a = 0, checksum_b = 0, checksum_c = 0, checksum_d = 0;

    // RANDOM
    start_a = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < ITERATIONS; i++)
    {
        checksum_a += encrypt(_rand_uint32(), subkeys, ENC_ROUNDS);
    }
    stop_a = std::chrono::high_resolution_clock::now();

    // COUNTER
    start_b = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < ITERATIONS; i++)
    {
        checksum_b += encrypt(i, subkeys, ENC_ROUNDS);
    }
    stop_b = std::chrono::high_resolution_clock::now();

    // PRIME-COUNTER
    pt = 0;
    start_c = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < ITERATIONS; i++)
    {
        checksum_c += encrypt(pt, subkeys, ENC_ROUNDS);
        pt += PRIME;
    }
    stop_c = std::chrono::high_resolution_clock::now();

    // RANDOM FROM FILE
    FILE *stream = fopen("/dev/urandom", "rb");
    uint32_t iters = 0x20000;
    char ptarr[4*iters];
    uint32_t *pts;
    start_d = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < ITERATIONS / iters; i++)
    {
        pts = (uint32_t *)fgets(ptarr, 4*iters, stream);
        for (uint64_t j = 0; j < iters; j ++)
        {
            checksum_d += encrypt(pts[j], subkeys, ENC_ROUNDS);
        }
    }
    stop_d = std::chrono::high_resolution_clock::now();
    fclose(stream);

    // Output results
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_a - start_a);
    std::cout << "Time A (mus): " << duration.count() << std::endl;
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_b - start_b);
    std::cout << "Time B (mus): " << duration.count() << std::endl;
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_c - start_c);
    std::cout << "Time C (mus): " << duration.count() << std::endl;
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_d - start_d);
    std::cout << "Time D (mus): " << duration.count() << std::endl;

    // Report any errors
    if (checksum_a != 576450066884551532)
    {
        std::cout << "INCORRECT A! found: " << checksum_a << std::endl;
    }
    if (checksum_b != 576481130847473411)
    {
        std::cout << "INCORRECT B! found: " << checksum_b << std::endl;
    }
    if (checksum_c != 576475346193833865)
    {
        std::cout << "INCORRECT C! found: " << checksum_c << std::endl;
    }
    std::cout << "D found: " << checksum_d << std::endl;
}

int main()
{
    testplaintextgen();
    return 0;
}