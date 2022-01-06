#pragma once

uint32_t encrypt(uint32_t word, uint16_t *subkeys, uint8_t rounds);

uint32_t decrypt(uint32_t word, uint16_t *subkeys, uint8_t rounds);
