#include "toolbox.h"
#include "aes.h"
#include <numeric>

uint8_t key_guesses_0x91[8] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t key_guesses_0xB3[8] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t key_guesses_0x22[8] = {0, 1, 8, 9, 64, 65, 72, 73};

const uint8_t masks_0x91[5] = {68, 111, 166, 201, 226};
const int8_t coeffs_0x91[5] = {-1, -1, -1, -1, -1};
const uint8_t offset_0x91 = 1;
const uint8_t masks_0xB3[5] = {10, 17, 198, 215, 221};
const int8_t coeffs_0xB3[5] = {-1, -1, -1, -1, -1};
const uint8_t offset_0xB3 = 1;
const uint8_t masks_0x22[5] = {19, 64, 152, 203, 216};
const int8_t coeffs_0x22[5] = {1, 1, 1, 1, 1};
const uint8_t offset_0x22 = 0;

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

inline uint16_t compute_output_sbox_parity(uint8_t pt, uint8_t key, const int8_t *coeffs, const uint16_t offset)
{
    int16_t linearized = 0;
    for (uint16_t ipm = 0; ipm < 0x100; ipm++)
    {
        linearized += coeffs[ipm] * P8((pt ^ key) & ipm);
    }
    return (uint16_t)(offset + (linearized >> 7));
}

inline uint16_t compute_input_sbox_parity(uint8_t pt, uint8_t key, const uint8_t *masks, const int8_t *coeffs, const uint8_t offset)
{
    int16_t linearized = 0;
    for (uint16_t i = 0; i < 5; i++)
    {
        linearized += coeffs[i] * P8((pt ^ key) & masks[i]);
    }
    return linearized >= 3 - 5 * offset;
}

void keyrecovery()
{
    // Settings
    const uint32_t sample_size = 0x4000000;
    uint32_t key[16] = {
        0xC4298E25,
        0x2DB74D07,
        0xF85B2ABC,
        0xD04C1C6F,

        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,

        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,

        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000};

    uint32_t pt[4];
    uint32_t counter = 0;
    uint32_t ipc, opc;
    uint32_t p0, p1, p2, p3;
    uint8_t k0, k1, k2, k3;

    for (uint32_t i = 0; i < sample_size; i++)
    {
        // Choose random plaintext
        for (uint16_t j = 0; j < 4; j++)
        {
            pt[j] = rand_uint32();
        }

        // Lookup key guess
        k0 = 0x00;
        k1 = 0x02;
        k2 = 0x09;
        k3 = 0x48;

        // Input correlation
        p0 = compute_input_sbox_parity(pt[0] >> 24 & 0xFF, k0, masks_0x91, coeffs_0x91, offset_0x91);
        p1 = compute_input_sbox_parity(pt[1] >> 16 & 0xFF, k1, masks_0xB3, coeffs_0xB3, offset_0xB3);
        p2 = compute_input_sbox_parity(pt[2] >> 8 & 0xFF, k2, masks_0x22, coeffs_0x22, offset_0x22);
        p3 = compute_input_sbox_parity(pt[3] >> 0 & 0xFF, k3, masks_0x22, coeffs_0x22, offset_0x22);
        ipc = p0 ^ p1 ^ p2 ^ p3;

        // Encrypt
        encrypt(pt, key, 3);

        // Output correlation
        p0 = compute_output_sbox_parity(pt[0] >> 24 & 0xFF, 0x00, COEFFS_O_0x2E, O_0x2E_offset);
        p1 = compute_output_sbox_parity(pt[3] >> 16 & 0xFF, 0x00, COEFFS_O_0xFA, O_0xFA_offset);
        p2 = compute_output_sbox_parity(pt[2] >> 8 & 0xFF, 0x00, COEFFS_O_0xB6, O_0xB6_offset);
        p3 = compute_output_sbox_parity(pt[1] >> 0 & 0xFF, 0x00, COEFFS_O_0x72, O_0x72_offset);
        opc = p0 ^ p1 ^ p2 ^ p3;

        counter += ipc ^ opc;
    }

    // Print results
    std::cout << counter << "/" << sample_size << std::endl;
}

int main()
{
    keyrecovery();
    return 0;
}