#pragma once
#include <iostream>

uint8_t SubByte(uint8_t word);

void encrypt(uint32_t * State, uint32_t * Key);

void ExpandKey(uint32_t * key, uint32_t * expandedkey, uint8_t nr_rounds);