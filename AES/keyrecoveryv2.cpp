#include "toolbox.h"
#include <numeric>
#include <algorithm>
#include "math.h"

// Coefficients corresponding with 0x91 as output mask
const int8_t COEFFS_I_0x91[256] = {0, 0, -10, 6, 2, -2, -4, -8, 14, -2, 0, 8, 4, 0, 2, 6, -12, -8, -10, -14, -2, -10, -4, 12, -6, 6, 8, 4, 8, 8, 10, -6, 12, 4, 10, 2, -6, -10, -12, 0, -2, 6, -8, 8, -8, 4, -10, -6, -8, 12, 2, -2, -2, -2, 12, -12, 10, 6, 0, -4, 12, 4, -2, -10, 2, 2, 4, 12, -16, -4, -2, 2, 0, -8, -2, -10, 2, 6, -12, 8, 6, -6, -4, 0, -12, -4, -2, -10, -12, 8, -10, 2, 6, -2, -12, 4, 2, 10, 4, 4, 12, -8, 2, -10, 12, -4, -6, -6, 2, 6, 12, -16, 14, 2, -12, 8, -8, 8, 10, -6, 0, 4, 2, 14, 6, 6, 12, -12, 4, -8, 14, 2, 6, 6, -12, -12, -6, -10, 0, 4, 0, 8, -14, 2, -12, 12, 2, -14, -10, 2, 0, -12, 2, -6, -4, 4, -8, -12, 6, 2, -4, 0, -2, 2, 2, 10, -16, -8, 6, 2, -12, -8, -8, 8, -6, -14, -4, -4, 2, -6, 2, -2, -4, 0, 6, -10, 8, 8, 8, -12, -10, 2, -10, -6, -12, 0, -12, -12, -2, 6, -4, -16, -10, 10, 14, 14, -4, -4, 6, 14, 0, 8, 12, 8, -6, 6, -4, -4, 2, 10, 6, -6, -8, 4, -14, 6, -16, 12, 4, -4, 6, 6, 4, 8, -2, 2, -14, -6, -8, 0, -14, 2, -4, 12, -4, -8, 2, 14, 12, -12, 2, -14, 2, 6, 12, 8};
const uint16_t I_0x91_offset = (1 - (std::accumulate(COEFFS_I_0x91, COEFFS_I_0x91 + 256, 0) >> 7)) >> 1;

// Coefficients corresponding with 0xB3 as output mask
const int8_t COEFFS_I_0xB3[256] = {0, 0, 6, -2, 4, 0, 2, 6, 2, 14, -16, -4, 14, -2, -4, -4, 12, -16, 2, 6, -4, 4, 10, 2, -6, -14, -8, 8, 2, -10, 0, -4, 14, 2, 8, -12, -14, 2, -12, 12, 12, -12, -10, -2, 8, -12, 2, -2, -10, -2, 0, 8, 6, 2, 8, -12, 0, -12, -6, 6, 8, 8, 2, 10, -2, -2, 12, -4, 2, -10, 0, 4, 0, -4, 14, 2, 12, 4, -14, 2, -10, 2, -12, -8, 14, 6, -4, 12, -4, -4, 10, -6, -4, 0, 2, -10, 8, -12, -6, -10, -12, -12, 6, -10, 14, 14, 8, 0, 2, -2, 4, -8, 4, 12, 6, 6, -12, -8, 6, 2, -2, 2, -8, 12, 6, -2, -8, 0, -4, -4, 10, -6, 0, -4, 6, 2, 2, -2, -8, -4, -10, 6, 12, -12, 4, -8, -6, 6, -12, -4, -14, 2, 6, -2, 4, -4, 6, -6, -12, -8, -10, 10, 0, 4, 2, 2, -12, -12, -8, 0, -14, 2, -12, 0, -2, 2, 2, 10, 4, 4, -6, -10, 4, 8, 0, -12, -14, 6, -8, 8, -6, -6, 6, 6, 4, 12, 2, 6, -16, -4, -4, -8, -6, -10, 8, 0, -2, -10, 2, -2, -8, 4, 2, 10, -8, -16, 12, -4, -14, -6, 12, -16, -6, -10, 12, 8, 6, 10, 8, -8, 2, -6, 6, 6, 8, 8, 2, 14, 12, -8, -12, 12, -10, -2, 4, 8, 6, -6, -14, -10, 12, 8, 2, -6, 4, -12};
const uint16_t I_0xB3_offset = (1 - (std::accumulate(COEFFS_I_0xB3, COEFFS_I_0xB3 + 256, 0) >> 7)) >> 1;

// Coefficients corresponding with 0x22 as output mask
const int8_t COEFFS_I_0x22[256] = {0, -4, 8, -12, -6, -2, -10, 10, 12, 12, 8, 8, 10, 10, -6, -6, 0, -12, -4, 16, -2, 10, 6, 2, 8, 0, -8, 0, -6, 2, 6, 14, -2, -2, -6, -6, 4, -12, 12, -4, -14, -2, 10, 6, -12, -8, -8, 12, 10, 10, 10, -6, 4, 4, 8, 8, -14, 14, 14, 10, 8, 12, -8, 12, 16, 4, -12, 8, -10, -6, -2, -14, -4, 12, 12, -4, -10, -2, 10, 2, 4, 8, 4, -8, -2, 2, 10, -2, -4, -4, -8, 8, 10, -14, 2, -6, 14, -2, -2, -2, 8, 0, 4, -4, 2, -10, -10, -6, -8, 12, -8, 12, 6, 14, -14, -6, 4, 4, 4, 4, -2, -6, -2, -6, 8, -12, 12, -8, 12, 12, -8, 0, -2, 6, -2, -2, -12, 0, 4, -8, 2, 14, 6, 10, 12, -12, -4, 12, -6, -6, 6, 14, 16, 4, 4, 0, -6, -2, -6, 6, 2, -2, -6, -2, 0, 12, -12, -8, -14, 2, 6, -2, 4, 12, -12, -12, -2, -6, -6, -2, -8, -12, -8, -4, 2, 2, -6, 2, 0, 8, 12, -4, 0, 8, -8, 8, -10, -2, -14, 2, 8, 4, -4, 16, -6, -2, -14, 14, -4, -12, 0, 0, -2, 6, -2, -2, 16, -4, 0, 4, -2, 2, -6, 6, 6, 2, 2, 6, 0, -12, -8, -12, 6, -2, 6, -10, 4, 12, 0, 0, 6, 10, -2, 10, 12, -8, -8, -4, 10, -6, -10, -2, -12, 4, 4, -4};
const uint16_t I_0x22_offset = (1 - (std::accumulate(COEFFS_I_0x22, COEFFS_I_0x22 + 256, 0) >> 7)) >> 1;

// Coefficients corresponding with 0x2E as input mask
const int8_t COEFFS_O_0x2E[256] = {0, -2, -6, -8, -2, -12, 8, 6, 0, 6, -14, 8, -2, -12, -8, 6, -12, -2, 10, 4, 6, 8, -12, -2, 4, -2, -6, 4, -2, -8, 4, 6, 10, -12, -8, -14, 0, 10, -2, 0, 6, -8, -12, -10, 4, -2, 10, -4, 10, 0, 12, -14, 4, -6, -2, -4, -10, -4, 0, 6, 0, -2, 10, 0, 12, 10, -2, -4, 2, 8, 12, -6, -16, 6, -6, -16, -2, -12, -8, -10, -12, 6, -6, -4, 6, 0, 12, 14, -8, -6, -2, 0, -6, 12, -8, 2, -10, 8, 12, 14, -4, -2, 10, 4, -2, 8, -12, -2, 4, 6, -6, 4, 2, -8, -12, -6, -12, -6, 6, -12, -6, 0, -12, 10, 4, -14, 6, -4, 0, -2, 10, 8, -14, 0, 4, -6, -8, -10, 2, 16, -14, 8, 12, -6, 12, -10, 2, 12, 10, -12, 0, 2, -12, 6, -14, 4, 2, 12, 8, 10, 2, 12, -8, 2, -4, -2, -6, 4, -2, -8, 12, 6, -8, 2, 6, -8, 2, 8, 12, 2, -8, 6, 2, -8, -2, -4, 8, 6, -4, 10, -2, 4, -16, -2, -14, -16, -6, -8, 12, 2, 12, -6, -2, -4, -10, -4, 0, -2, -8, 10, -2, 0, 6, -8, -12, 14, -12, -2, -14, -4, -6, -4, 8, 2, 10, 12, 8, -6, -12, 14, 2, -12, 2, -12, 8, -6, 4, 6, -14, -4, -2, 4, -8, -2, -4, 10, -2, 4, -10, 4, 0, 14, 4, 2, 14, 4};
const uint16_t O_0x2E_offset = (1 - (std::accumulate(COEFFS_O_0x2E, COEFFS_O_0x2E + 256, 0) >> 7)) >> 1;

// Coefficients corresponding with 0xFA as input mask
const int8_t COEFFS_O_0xFA[256] = {0, 14, 2, 12, -10, -8, 0, -2, 6, 12, 0, -14, -4, -2, 6, 4, 2, -4, 0, -2, -4, -14, 2, -4, 4, 14, 2, 16, 14, 12, 12, -2, -12, 2, -10, 8, 2, -12, 12, 2, -6, -8, 12, 14, 8, -14, -6, -8, -6, -4, -8, -10, -12, 2, 10, -12, -12, -2, 10, 0, -2, 12, -12, -2, 6, 8, -12, 2, 4, -6, 2, 4, -16, 2, 6, -12, -10, 4, 4, -2, -4, 2, 6, -16, 6, 8, 0, 6, -6, 8, 4, -10, -4, -2, 14, 4, 6, 8, -12, 10, -12, -6, 2, -4, 8, 2, 6, -12, 14, -12, 4, 14, -8, 6, -14, -4, 10, 4, -12, 10, 6, 4, 8, -14, 0, 2, -6, -8, 6, -4, 0, -14, 4, 6, -10, 4, 4, 2, -10, 16, -6, -4, -4, -6, -8, 2, -10, 4, 2, 0, -8, 10, 2, 12, 0, -2, 4, 10, 2, -4, 2, 8, -12, -2, -8, 10, 2, 8, 8, -2, -6, 4, 6, 0, -8, 6, -8, -6, 14, 12, 2, -8, 0, 2, 2, 12, 0, 6, -12, -6, -14, -12, -12, 6, -6, 8, 2, 0, 8, -14, 6, 8, 4, 2, 4, 10, -6, -4, -6, 8, -12, -10, -4, -2, -10, -4, -16, 6, -6, -12, 2, -12, -12, 10, 12, -2, 10, 0, -6, -8, 8, -6, -2, -8, 12, -6, -4, -6, 2, -12, -2, -12, 0, 2, -8, 2, -6, 0, 12, 2, -10, -8, -10, 8, -8, -10};
const uint16_t O_0xFA_offset = (1 - (std::accumulate(COEFFS_O_0xFA, COEFFS_O_0xFA + 256, 0) >> 7)) >> 1;

// Coefficients corresponding with 0xB6 as input mask
const int8_t COEFFS_O_0xB6[256] = {0, 4, 6, -2, -12, 0, -10, 6, 0, 12, 10, 2, 16, -4, -2, 6, -8, -8, 6, -6, 0, 0, 2, 6, 12, 4, 6, 10, -8, -8, -2, -6, -14, -14, -8, 4, 14, -2, -8, -12, -6, -6, -12, 8, 2, 10, -8, -12, 6, -6, 4, -12, -2, -6, 8, 0, -6, -2, -12, -4, -10, 10, 4, -4, 6, 6, -12, 8, -6, 2, 12, 8, -6, 10, -4, 8, 10, 10, -8, -12, 10, -2, 8, 0, 10, -2, 4, -4, 2, 6, 12, -4, -10, -14, -12, -4, 8, -4, 14, -2, -4, -16, -2, -2, 4, -16, 14, 14, 4, -8, 2, -6, 8, -8, 14, -6, 0, 8, 10, -2, 0, 8, 2, -2, 12, 4, -6, -10, 12, -8, 10, -6, -12, 8, 6, -2, -8, 12, 2, 2, 12, 0, 2, 2, -4, -4, -6, 14, 0, 0, -6, 14, 4, -4, 6, -6, -4, -4, 2, -2, 2, 10, -8, -4, 2, -6, -12, -8, -2, 6, 0, -4, 10, -6, 16, 4, 14, 2, -12, 4, 2, -2, -4, 4, -2, 2, 8, 0, -10, 10, 12, 4, 2, -6, 0, -4, -6, 10, 4, 8, 2, -6, -4, -16, -10, 14, -12, -8, 6, 10, 12, -12, -14, 6, 12, -12, 2, 6, 12, 12, 2, 14, 8, 0, -8, -12, -10, -2, 0, 12, 2, -6, 8, 12, 2, -6, 12, 8, -14, 2, 8, -8, -2, 10, 12, 4, -2, -14, 12, 4, 6, 2, -12, 12, 2, -2};
const uint16_t O_0xB6_offset = (1 - (std::accumulate(COEFFS_O_0xB6, COEFFS_O_0xB6 + 256, 0) >> 7)) >> 1;

// Coefficients corresponding with 0x72 as input mask
const int8_t COEFFS_O_0x72[256] = {0, 4, 8, 12, -2, 6, -6, -6, 4, -4, 4, -4, -2, 2, 10, 6, 10, -14, 2, 10, -12, 0, 0, -12, -6, 6, 2, -2, 0, -8, 4, -12, 10, -2, -14, -2, -4, 4, 0, 8, -6, 10, -6, 2, -8, -12, 12, 8, 4, 4, 12, 4, 10, -2, -2, 2, -16, -4, -8, 12, 2, -6, -2, -10, -16, 4, -12, 8, -10, -2, 6, 6, -4, 4, 8, 0, -2, -14, -2, -6, 6, -2, 2, -6, -8, 4, 0, -12, -2, 10, -6, 6, 12, 4, 12, 12, -2, -6, -6, -2, 8, -8, -8, -8, -2, 6, 2, 2, -12, -8, -12, -8, -4, -12, 0, 0, 10, -10, -14, -2, -8, -4, 12, -8, -14, -14, 2, 2, 10, 10, 6, -10, -8, 12, -16, -4, -6, -10, 6, 2, 12, -12, -4, 12, 8, -12, -4, 8, 10, -6, -6, 2, 12, -12, -8, 0, 2, 6, 2, -2, -8, -8, -12, 12, -6, 14, -6, -2, 4, -8, 0, -4, 10, -6, -14, 2, -10, -6, -6, 6, 4, 12, -4, -12, 6, -2, 2, 2, 8, 12, 16, 4, 14, -2, 6, -10, -12, -8, 16, -4, 6, -14, 14, -6, 0, 8, 4, -12, -8, -12, 0, 12, -14, -14, -2, 6, 4, 12, 4, 12, 2, -10, 14, -6, 0, 8, 0, 0, 10, -10, -10, -14, -4, 8, -4, 0, -6, 2, -2, -10, 2, -2, 2, 6, 8, -8, 4, 4, 2, -14, -6, 2, 12, 8, 8, 4};
const uint16_t O_0x72_offset = (1 - (std::accumulate(COEFFS_O_0x72, COEFFS_O_0x72 + 256, 0) >> 7)) >> 1;


uint16_t guess_parity_sbox(uint8_t pt, uint8_t key_guess, const int8_t *coeffs, const uint16_t offset)
{
    int16_t linearized = 0;
    for (uint16_t ipm = 0; ipm < 0x100; ipm++)
    {
        linearized += coeffs[ipm] * P8((pt ^ key_guess) & ipm);
    }
    return (uint16_t) (offset + (linearized >> 7));
}

uint32_t bit_count(uint32_t word)
{
    uint32_t count = 0;
    for (uint64_t i = 0; i < 32; i++)
    {
        count += ((word & (1 << i)) > 0);
    }
    return count;
}

uint32_t guess_key(const uint32_t *key)
{
    // Test settings
    const uint32_t sample_size = 0x400;

    // Initialize plaintexts and cipher texts
    uint32_t pt[sample_size];
    uint32_t ct[sample_size];
    for (uint32_t i = 0; i < sample_size; i++)
    {
        pt[i] = rand_uint32();
        ct[i] = MixColumn(SubByteCol(pt[i] ^ key[0]));  // First round
        ct[i] = MixColumn(SubByteCol(ct[i] ^ key[1]));  // Second round
        ct[i] = key[3] ^ SubByteCol(ct[i] ^ key[2]);  // Third round
    }

    // We allow ourselves to know some key bytes
    uint8_t ik0 = key[0] >> 24 & 0xFF;
    uint8_t ik1 = key[0] >> 16 & 0xFF;
    uint8_t ik2 = key[0] >>  8 & 0xFF;
    uint8_t ik3 = key[0] >>  0 & 0xFF;
    uint8_t key0_guess[4] = {0, 0, 0, 0};
    uint8_t ok0 = key[3] >> 24 & 0xFF;
    uint8_t ok1 = key[3] >> 16 & 0xFF;
    uint8_t ok2 = key[3] >>  8 & 0xFF;
    uint8_t ok3 = key[3] >>  0 & 0xFF;
    uint8_t key3_guess[4] = {0, 0, 0, 0};

    // Initialize coefficients
    int8_t parity_key0_b0[sample_size];
    int8_t parity_key0_b1[sample_size];
    int8_t parity_key0_b2[sample_size];
    int8_t parity_key0_b3[sample_size];
    int8_t parity_key3_b0[sample_size];
    int8_t parity_key3_b1[sample_size];
    int8_t parity_key3_b2[sample_size];
    int8_t parity_key3_b3[sample_size];
    for (uint32_t i = 0; i < sample_size; i++)
    {
        parity_key0_b0[i] = guess_parity_sbox(pt[i] >> 24 & 0xFF, key0_guess[0], COEFFS_I_0x91, I_0x91_offset);
        parity_key0_b1[i] = guess_parity_sbox(pt[i] >> 16 & 0xFF, key0_guess[1], COEFFS_I_0xB3, I_0xB3_offset);
        parity_key0_b2[i] = guess_parity_sbox(pt[i] >>  8 & 0xFF, key0_guess[2], COEFFS_I_0x22, I_0x22_offset);
        parity_key0_b3[i] = guess_parity_sbox(pt[i] >>  0 & 0xFF, key0_guess[3], COEFFS_I_0x22, I_0x22_offset);

        parity_key3_b0[i] = guess_parity_sbox(ct[i] >> 24 & 0xFF, key3_guess[0], COEFFS_O_0x2E, O_0x2E_offset);
        parity_key3_b1[i] = guess_parity_sbox(ct[i] >> 16 & 0xFF, key3_guess[1], COEFFS_O_0xFA, O_0xFA_offset);
        parity_key3_b2[i] = guess_parity_sbox(ct[i] >>  8 & 0xFF, key3_guess[2], COEFFS_O_0xB6, O_0xB6_offset);
        parity_key3_b3[i] = guess_parity_sbox(ct[i] >>  0 & 0xFF, key3_guess[3], COEFFS_O_0x72, O_0x72_offset);
    }

    // Guess key
    int32_t count;
    uint8_t in_parity, out_parity;
    for (uint16_t kg = 0; kg < 0x100; kg ++)
    {
        count = sample_size / 2;
        for (uint32_t i = 0; i < sample_size; i++)
        {
            in_parity = 0;
            in_parity ^= parity_key0_b0[i];
            in_parity ^= parity_key0_b1[i];
            in_parity ^= parity_key0_b2[i];
            // in_parity ^= parity_key0_b3[i];
            in_parity ^= guess_parity_sbox(pt[i] >>  0 & 0XFF, kg, COEFFS_I_0x22, I_0x22_offset);

            out_parity = 0;
            out_parity ^= parity_key3_b0[i];
            out_parity ^= parity_key3_b1[i];
            out_parity ^= parity_key3_b2[i];
            // out_parity ^= guess_parity_sbox(ct[i] >> 8 & 0xFF, kg, COEFFS_O_0xB6, O_0xB6_offset);
            out_parity ^= parity_key3_b3[i];

            count -= in_parity ^ out_parity;
        }
    }
    std::cout << std::endl;

    // for (uint16_t kg = 0; kg < 0x100; kg ++)
    // {
    //     count = sample_size / 2;
    //     for (uint32_t i = 0; i < sample_size; i++)
    //     {
    //         in_parity = 0;
    //         in_parity ^= parity_key0_b0[i];
    //         in_parity ^= parity_key0_b1[i];
    //         // in_parity ^= parity_key0_b2[i];
    //         in_parity ^= guess_parity_sbox(pt[i] >>  8 & 0XFF, kg, COEFFS_I_0x22, I_0x22_offset);
    //         in_parity ^= parity_key0_b3[i];
    //         // in_parity ^= guess_parity_sbox(pt[i] >>  0 & 0XFF, kg, COEFFS_I_0x22, I_0x22_offset);

    //         out_parity = 0;
    //         out_parity ^= parity_key3_b0[i];
    //         out_parity ^= parity_key3_b1[i];
    //         out_parity ^= parity_key3_b2[i];
    //         // out_parity ^= guess_parity_sbox(ct[i] >> 8 & 0xFF, kg, COEFFS_O_0xB6, O_0xB6_offset);
    //         out_parity ^= parity_key3_b3[i];

    //         count -= in_parity ^ out_parity;
    //     }
    //     std::cout << "(" << kg << ", " << count << "), ";
    // }
    // std::cout << std::endl;


    return 0;
}


int main()
{
    const uint32_t key[4] = {
        // 0x9926A6A8,
        // 0x9926B3F5,
        // 0x2F43D718,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        // 0x6487F2D3
    };
    guess_key(key);

    return 0;
}