#pragma once
#include <string>

void generateSubKeys(uint64_t key, uint16_t *subkeys, uint8_t rounds);

// Computes parity of 32-bit word.
uint32_t P32(uint32_t v);

// Generate filename
std::string generateFileName(std::string name, uint32_t inp_mask,
                             uint32_t out_mask, uint32_t sample_size,
                             uint32_t nr_trials, uint8_t enc_rounds);

// Generate random 32-bit word.
uint32_t rand_uint32();

// Generate random 64-bit word.
uint64_t rand_uint64();
