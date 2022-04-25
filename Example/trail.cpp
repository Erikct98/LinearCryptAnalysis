#include <iostream>
#include <random>
#include <fstream>
#include "AESesque.h"
#include "toolbox.h"

// Global settings
static const uint32_t NR_ITERATIONS = 0x10;     // 16
static const uint32_t SAMPLE_SIZE = 0x80000000; // 2^31
static const uint32_t ROUNDS = 4;
static const uint32_t IPM[4] = {
    0x09990000,
    0x00077000,
    0x000F0F00,
    0x0000DD00};
static const uint32_t OPM[4] = {
    0x00000A6D,
    0x00000530,
    0x00000A07,
    0x000000CB};
static uint32_t CONSTANTS[4] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};
// static uint32_t CONSTANTS[4] = {
//     0x47AEB5C6,
//     0xE6BA6404,
//     0x01E1A321,
//     0x4E9CED5D
// };

// Randomization
std::mt19937 mt(time(0));
std::uniform_int_distribution<uint32_t> randint(0, 0xFFFFFFFF);

// Count number of times Linear Approximation is CORRECT
uint32_t count(uint32_t *key, uint32_t *constants, uint32_t sample_size)
{
    uint32_t word[4];
    uint32_t count = sample_size;
    uint32_t val;

    for (uint32_t i = 0; i < sample_size; i++)
    {
        // Generate random input
        val = 0;
        for (uint8_t i = 0; i < 4; i++)
        {
            word[i] = randint(mt);
            val ^= word[i] & IPM[i];
        }

        // Encrypt
        encrypt(word, key, constants, ROUNDS);

        // XOR parity output
        for (uint8_t i = 0; i < 4; i++)
        {
            val ^= word[i] & OPM[i];
        }

        // Analyse
        count -= P32(val);
    }

    return count;
}

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
    uint32_t key[4] = {0};
    uint32_t counts[NR_ITERATIONS];
    for (uint8_t i = 0; i < NR_ITERATIONS; i++)
    {
        counts[i] = count(key, CONSTANTS, SAMPLE_SIZE);
        std::cout << counts[i] << std::endl;

        for (uint8_t j = 0; j < 4; j++)
        {
            key[j] = randint(mt);
        }
    }

    writeToFile(counts, NR_ITERATIONS, "results/trail_results.dat");

    return 0;
}