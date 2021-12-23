#pragma once
#include <iostream>

void encrypt(uint64_t *word, uint64_t *res);

uint64_t generateRandom64Bits();

uint64_t getParity(uint64_t v);