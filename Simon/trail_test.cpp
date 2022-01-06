#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include "simon.h"
#include "toolbox.h"

void compute_correlation(uint32_t inp_mask, uint32_t out_mask, uint32_t sample_size, uint32_t nr_trials, uint8_t enc_rounds, uint32_t *results)
{
    // Constants
    uint64_t key;
    uint32_t pt, ct;
    uint16_t subkeys[enc_rounds];
    for (int i = 0; i < nr_trials; i++)
    {
        // Generate subkeys
        key = rand_uint64();
        generateSubKeys(key, subkeys, enc_rounds);

        // Compute correlation
        for (int j = 0; j < sample_size; j++)
        {
            pt = rand_uint32();
            ct = encrypt(pt, subkeys, enc_rounds);
            results[i] += getParity((inp_mask & pt) ^ (out_mask & ct));
        }
    }
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

    // Settings

    // The infamous 9-round for 2^-10
    // uint32_t inp_mask = 0x40040001, out_mask = 0x00014004; uint8_t enc_rounds = 5; // {2, 14}, {0} -> {0}, {2, 14} // 5 rounds
    // uint32_t inp_mask = 0x40040001, out_mask = 0x40041000; uint8_t enc_rounds = 6; // {2, 14}, {0} -> {2, 14}, {12} // 6 rounds
    // uint32_t inp_mask = 0x40040001, out_mask = 0x10004404; uint8_t enc_rounds = 7; // {2, 14}, {0} -> {12}, {2, 10, 14} // 7 rounds
    // uint32_t inp_mask = 0x00014004, out_mask = 0x10004404; uint8_t enc_rounds = 2; // {0}, {2, 14} -> {12}, {2, 10, 14} // 2 rounds
    // uint32_t inp_mask = 0x00014004, out_mask = 0x40041000; uint8_t enc_rounds = 1; // {0}, {2, 14} -> {2,14}, {12} // 1 round

    // Ashur 2016
    // uint32_t inp_mask = 0x05001001, out_mask = 0x00010000; uint8_t enc_rounds = 3; // {8, 10}, {0, 12} -> {}, {0} // 3 rounds
    uint32_t inp_mask = 0x00204080, out_mask = 0x40801000; uint8_t enc_rounds = 1; // {5}, {7, 14} -> {7, 14}, {12} // 1 rounds
    // uint32_t inp_mask = 0x00404080, out_mask = 0x40801020; uint8_t enc_rounds = 1; // {6}, {7, 14} -> {7, 14}, {5, 12} // 1 rounds

    uint32_t nr_trials = 0x4000;
    for (uint32_t sample_size = 0x10000; sample_size <= 0x10000; sample_size <<= 1)
    {
        uint32_t results[nr_trials] = {0};
        compute_correlation(inp_mask, out_mask, sample_size, nr_trials, enc_rounds, results);
        std::string fname = generateFileName("results/trail_test", inp_mask, out_mask, sample_size, nr_trials, enc_rounds);
        std::cout << fname << '\n';
        writeToFile(results, nr_trials, fname);
    }

    return 0;
}