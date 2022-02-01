#pragma once
#include <iostream>

// Section-Wise (8 bits) CYclic LEft SHift uint64_t's over by one bit.
#define sw8cylesh32_1(word) (((word & 0x7F7F7F7F) << 1 ^ (word & 0x80808080) >> 7))
#define sw8cylesh64_1(word) ((word & 0x7F7F7F7F7F7F7F7F) << 1 ^ (word & 0x8080808080808080) >> 7)

// CYclic LEft SHift uint32_t's by X bits
#define cylesh32_8(word) (word << 8 ^ word >> 24)
#define cylesh32_16(word) (word << 16 ^ word >> 16)
#define cylesh32_24(word) (word << 24 ^ word >> 8)

// Compute parity of 32-bit word.
uint32_t P32(uint32_t v);

uint32_t mulp2(uint32_t a);

uint8_t SubByteByte(uint8_t word);

uint32_t SubByteCol(uint32_t col);

void SubBytes(uint32_t *state);

void ShiftRows(uint32_t *state);

uint32_t MixColumn(uint32_t state);

void MixColumns(uint32_t *state);

void AddRoundKey(uint32_t *state, uint32_t *ExpandedKey, uint8_t round);

void ExpandKey(uint32_t *key, uint32_t *expandedkey, uint8_t nr_rounds);

uint32_t rand_uint32();

void printResults(uint64_t *counts, uint16_t count_len, uint64_t total);
