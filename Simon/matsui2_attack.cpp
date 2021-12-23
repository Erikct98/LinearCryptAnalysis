#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include "simon.h"
#include "encrypt.h"

void attack(uint32_t nr_samples, uint16_t *subkeys, uint8_t rounds, uint32_t ipm, uint32_t imm, uint32_t opm, int32_t *counts)
{
    uint16_t decrypt_key;
    uint32_t ct, parity, dt;
    for (uint32_t pt = 0; pt < nr_samples; pt++)
    {
        ct = encrypt(pt, subkeys, rounds);
        for (uint32_t j = 0; j < 4; j++)
        {
            // decrypt_key = ((j << 2) & 0x0004) ^ ((j << 13) & 0x4000);
            decrypt_key = j << 12;
            dt = decrypt(ct, &decrypt_key, 1);
            counts[j] += getParity((pt & ipm) ^ (dt & imm)) ^ 1;
        }
    }
}

int main()
{
    std::srand(time(0));

    // Settings
    uint32_t correct_count = 0;
    uint32_t rounds = 9;
    uint32_t sample_size = 0x400000; // 2^22 = 4/(2^-10)^2 -> should give 99.9-ish% success rate.
    uint32_t ipm = 0x44041000, imm = 0x40041000, opm = 0x10004404;
    uint32_t trials = 0x40;

    uint8_t nr_keys = 2;

    uint64_t key;
    uint32_t guess;
    for (uint32_t i = 0; i < trials; i++)
    {
        // Generate subkeys
        key = getRandomKey();
        uint16_t subkeys[rounds] = {0};
        generateSubKeys(key, subkeys, rounds);

        // Perform encryptions
        int32_t counts[nr_keys] = {0};
        attack(sample_size, subkeys, rounds, ipm, imm, opm, counts);

        // // Print counts to file
        // std::string fname = generateFileName("matsui2_attack", ipm, opm, sample_size, (uint32_t)0, rounds);
        // writeToFile(counts, 2, fname);

        // Print results
        uint32_t keybit = (subkeys[rounds-1] >> 12) & 1;
        std::cout << keybit << ' ';
        for (uint8_t j = 0; j < nr_keys; j++) {
            counts[j] = counts[j] - (sample_size >> 1);
            std::cout << counts[j] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}