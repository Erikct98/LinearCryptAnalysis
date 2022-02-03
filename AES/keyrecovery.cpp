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

uint16_t guess_parity_sbox_0(uint8_t pt, uint8_t key_guess)
{
    int16_t linearized = 0;
    for (uint16_t ipm = 0; ipm < 0x100; ipm++)
    {
        linearized += COEFFS_I_0x91[ipm] * P8((pt ^ key_guess) & ipm);
    }
    return (uint16_t) (I_0x91_offset + (linearized >> 7));
}

uint16_t guess_parity_sbox_1(uint8_t pt, uint8_t key_guess)
{
    int16_t linearized = 0;
    for (uint16_t ipm = 0; ipm < 0x100; ipm++)
    {
        linearized += COEFFS_I_0xB3[ipm] * P8((pt ^ key_guess) & ipm);
    }
    return (uint16_t) (I_0xB3_offset + (linearized >> 7));
}

uint16_t guess_parity_sbox_23(uint8_t pt, uint8_t key_guess)
{
    int16_t linearized = 0;
    for (uint16_t ipm = 0; ipm < 0x100; ipm++)
    {
        linearized += COEFFS_I_0x22[ipm] * P8((pt ^ key_guess) & ipm);
    }
    return (uint16_t) (I_0x22_offset + (linearized >> 7));
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

uint32_t guess_key(const uint32_t key)
{
    // Test settings
    const uint64_t sample_size = 0x400;
    const uint16_t nr_improvement_iterations = 0x5;
    const uint32_t OPM = 0x22000000;

    // Initialize plaintexts
    uint32_t pt[sample_size];
    uint32_t ct_par[sample_size];

    // Initialize plaintext and ciphertext
    for (uint16_t i = 0; i < sample_size; i++)
    {
        pt[i] = rand_uint32();
        ct_par[i] = P32(MixColumn(SubByteCol(pt[i] ^ key)) & OPM);
    }

    // Initialize guesses
    uint8_t guess_box_0[sample_size];
    uint8_t guess_box_1[sample_size];
    uint8_t guess_box_2[sample_size];
    uint8_t guess_box_3[sample_size];

    // Other variables
    int16_t count;
    uint16_t in_parity;
    uint16_t out_parity;
    uint32_t res;

    // Recover key
    uint32_t guess;
    uint16_t key_guess[4] = {0x99, 0x26, 0, 0};
    uint16_t arg[4] = {0,0,0,0};
    uint16_t argmax[4] = {0,0,0,0};
    int16_t all_time_guess[32];
    for (uint16_t i = 0 ; i < 32; i ++)
    {
        all_time_guess[i] = nr_improvement_iterations / 2;
    }


    // Iteratively improve guess
    for (uint16_t it = 0; it < nr_improvement_iterations; it++)
    {
        // Setup initial guess.
        for (uint16_t i = 0; i < sample_size; i++)
        {
            guess_box_0[i] = guess_parity_sbox_0(pt[i] >> 24 & 0xFF, key_guess[0]);
            guess_box_1[i] = guess_parity_sbox_1(pt[i] >> 16 & 0xFF, key_guess[1]);
            guess_box_2[i] = guess_parity_sbox_23(pt[i] >> 8 & 0xFF, key_guess[2]);
            guess_box_3[i] = guess_parity_sbox_23(pt[i] & 0xFF, key_guess[3]);
        }

        // Make key guess for first box
        arg[0] = 0;
        argmax[0] = 0;
        for (uint16_t kg = 0; kg < 0x100; kg ++)
        {
            // Compute correlation in case of this key.
            count = sample_size / 2;
            for (uint16_t j = 0; j < sample_size; j++)
            {
                in_parity = guess_parity_sbox_0(pt[j] >> 24 & 0xFF, kg);
                in_parity ^= guess_box_1[j] ^ guess_box_2[j] ^ guess_box_3[j];
                count -= in_parity ^ ct_par[j];
            }

            // Check if this guess is better than current
            // std::cout << "(" << kg << ", " << std::abs(count) << ", " << count << "), ";
            if (std::abs(count) > argmax[0])
            {
                arg[0] = kg;
                argmax[0] = std::abs(count);
            }
        }
        // Update key
        // std::cout << "\nupdating to " << std::hex<< arg << std::endl;

        // Make key guess for second box
        arg[1] = 0;
        argmax[1] = 0;
        for (uint16_t kg = 0; kg < 0x100; kg ++)
        {
            // Compute correlation in case of this key.
            count = sample_size / 2;
            for (uint16_t j = 0; j < sample_size; j++)
            {
                in_parity = guess_parity_sbox_1(pt[j] >> 16 & 0xFF, kg);
                in_parity ^= guess_box_0[j] ^ guess_box_2[j] ^ guess_box_3[j];
                count -= in_parity ^ ct_par[j];
            }

            // Check if this guess is better than current
            // std::cout << "(" << key << ", " << std::abs(count) << ", " << count << "), ";
            if (std::abs(count) > argmax[1])
            {
                // std::cout << "Improvement!: 0x" << std::hex << key << ", 0x" << arg << std::endl;
                arg[1] = kg;
                argmax[1] = std::abs(count);
            }
        }
        // Update key

        // Make key guess for third box
        arg[2] = 0;
        argmax[2] = 0;
        for (uint16_t kg = 0; kg < 0x100; kg ++)
        {
            // Compute correlation in case of this key.
            count = sample_size / 2;
            for (uint16_t j = 0; j < sample_size; j++)
            {
                in_parity = guess_parity_sbox_23(pt[j] >> 8 & 0xFF, kg);
                in_parity ^= guess_box_0[j] ^ guess_box_1[j] ^ guess_box_3[j];
                count -= in_parity ^ ct_par[j];
            }

            // Check if this guess is better than current
            if (std::abs(count) > argmax[2])
            {
                arg[2] = kg;
                argmax[2] = std::abs(count);
            }
        }

        // Make key guess for fourth box
        arg[3] = 0;
        argmax[3] = 0;
        for (uint16_t kg = 0; kg < 0x100; kg ++)
        {
            // Compute correlation in case of this key.
            count = sample_size / 2;
            for (uint16_t j = 0; j < sample_size; j++)
            {
                in_parity = guess_parity_sbox_23(pt[j] & 0xFF, kg);
                in_parity ^= guess_box_0[j] ^ guess_box_1[j] ^ guess_box_2[j];
                count -= in_parity ^ ct_par[j];
            }

            // Check if this guess is better than current
            if (std::abs(count) > argmax[3])
            {
                arg[3] = kg;
                argmax[3] = std::abs(count);
            }
        }

        // Update key guess
        // for (uint16_t i = 0 ; i < 4; i++)
        // {
        //     std::cout << arg[i] << " " << argmax[i] << std::endl;
        // }

        uint16_t *max = std::max_element(argmax, argmax + 4);
        int offset = max - argmax;
        key_guess[offset] = arg[offset];

        guess = key_guess[0] << 24 ^ key_guess[1] << 16 ^ key_guess[2] << 8 ^ key_guess[3];
        // std::cout << std::hex << guess << ", " << std::dec<< bit_count(guess ^ key) << std::endl;

        for (uint16_t i = 0; i < 32; i ++)
        {
            all_time_guess[i] -= (guess >> i) & 0x1;
        }
    }

    uint32_t final_guess = 0;
    // for (uint16_t i = 0 ; i < 32; i ++)
    // {
    //     std::cout << all_time_guess[i] << ", ";
    //     final_guess ^= (all_time_guess[i] > 0) << i;
    // }

    std::cout << "final guess: " << std::hex << guess << std::endl;
    return guess;
}

void test(const uint32_t key, const uint32_t key_guess)
{
     // Test settings
    const uint64_t sample_size = 0x400;
    const uint16_t nr_improvement_iterations = 0x5;
    const uint32_t OPM = 0x22000000;

    // Initialize plaintexts
    uint32_t pt[sample_size];
    uint32_t ct_par[sample_size];

    // Initialize plaintext and ciphertext
    for (uint16_t i = 0; i < sample_size; i++)
    {
        pt[i] = rand_uint32();
        ct_par[i] = P32(MixColumn(SubByteCol(pt[i] ^ key)) & OPM);
    }
    uint32_t in_parity;

    // Setup initial guess.
    int count = sample_size;
    for (uint16_t i = 0; i < sample_size; i++)
    {
        in_parity = guess_parity_sbox_0(pt[i] >> 24 & 0xFF, key_guess >> 24 & 0xFF)
                    ^ guess_parity_sbox_1(pt[i] >> 16 & 0xFF, key_guess >> 16 & 0xFF)
                    ^ guess_parity_sbox_23(pt[i]>> 8 & 0xFF, key_guess >> 8 & 0xFF)
                    ^ guess_parity_sbox_23(pt[i]>> 0 & 0xFF, key_guess & 0xFF);
        count -= in_parity ^ ct_par[i];
    }

    float corr = 2 * (count / (float) sample_size) - 1;
    std::cout << "corr = 2 * (" << count << "/" << sample_size
              << ") - 1 = " << corr
              << " = (+/-) 2^" << std::log2(std::abs(corr)) << std::endl;
}

int main()
{
    const uint32_t key = 0x9926A6A8;
    const uint32_t key_guess = guess_key(key);
    test(key, key_guess);

    return 0;
}