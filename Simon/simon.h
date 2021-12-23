#pragma once
#include <iostream>

uint8_t Z[] = {1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0,
               0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1,
               0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0};

// CYclic RIght SHift
uint16_t cyrish(uint16_t word, uint8_t ticks)
{
    return (word >> ticks) ^ (word << (16 - ticks));
}

uint16_t *generateSubKeys(uint64_t key, uint16_t *subkeys, uint8_t rounds)
{
    // Break key in parts
    for (uint8_t i = 0; i < std::min((uint8_t) 4, rounds); i++)
    {
        subkeys[i] = (key >> (i << 4)) & 0xFFFF;
    }

    // Generate remaining subkeys
    uint16_t subkey;
    for (uint8_t i = 4; i < rounds; i++)
    {
        subkey = cyrish(subkeys[i - 1], 3);
        subkey ^= subkeys[i - 3];
        subkey ^= cyrish(subkey, 1) ^ subkeys[i - 4];
        subkey ^= 0xFFFC ^ Z[(i - 4) % 62];
        subkeys[i] = subkey;
    }
    return subkeys;
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

uint64_t getRandomKey()
{
    uint64_t key = rand();
    key = key << 31 ^ rand();
    key = key << 31 ^ rand();
    return key;
}

uint32_t getRandomPt()
{
    return (rand() << 31) ^ rand();
}

std::string generateFileName(std::string name, uint32_t inp_mask, uint32_t out_mask, uint32_t sample_size, uint32_t nr_trials, uint8_t enc_rounds)
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