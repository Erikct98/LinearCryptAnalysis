#include "toolbox.h"
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

// CYclic RIght SHift
#define cyrish_1(word) (word >> 1 ^ word << 15)
#define cyrish_3(word) (word >> 3 ^ word << 13)

static const uint8_t Z[] = {
    1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1,
    0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0};

void generateSubKeys(uint64_t key, uint16_t *subkeys, uint8_t rounds)
{
    // Break key in parts
    for (uint8_t i = 0; i < std::min(rounds, (uint8_t) 0x4); i++)
    {
        subkeys[i] = (key >> (i << 4)) & 0xFFFF;
    }

    // Generate remaining subkeys
    uint16_t subkey;
    for (uint8_t i = 4; i < rounds; i++)
    {
        subkey = cyrish_3(subkeys[i - 1]);
        subkey ^= subkeys[i - 3];
        subkey ^= cyrish_1(subkey) ^ subkeys[i - 4];
        subkey ^= 0xFFFC ^ Z[(i - 4) % 62];
        subkeys[i] = subkey;
    }
}

// Found on https://graphics.stanford.edu/~seander/bithacks.html
uint32_t getParity(uint32_t v)
{
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v &= 0xf;
    return (0x6996 >> v) & 1;
}

std::string generateFileName(std::string name, uint32_t inp_mask,
                             uint32_t out_mask, uint32_t sample_size,
                             uint32_t nr_trials, uint8_t enc_rounds)
{
    // Construct file names from details
    const auto p1 = std::chrono::system_clock::now();
    auto unix_time = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
    std::string fname = name.append("/results_r=")
                            .append(std::to_string(enc_rounds))
                            .append("_ipm=" + std::to_string(inp_mask))
                            .append("_opm=" + std::to_string(out_mask))
                            .append("_ssize=" + std::to_string(sample_size))
                            .append("_keys=" + std::to_string(nr_trials))
                            .append("_" + std::to_string(unix_time))
                            .append(".txt");
    return fname;
}

// Randomization
std::mt19937 mt(time(0));
std::uniform_int_distribution<uint32_t> randuint32(0, 0xFFFFFFFF);
std::uniform_int_distribution<uint64_t> randuint64(0, 0xFFFFFFFFFFFFFFFF);

uint32_t rand_uint32()
{
    return randuint32(mt);
}

uint64_t rand_uint64()
{
    return randuint64(mt);
}
