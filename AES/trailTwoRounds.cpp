#include <iostream>
#include "aes.h"
#include "toolbox.h"
#include "chrono"
#include <random>
#include <numeric>

void trailTwoRounds()
{
    // Parallelization settings
    uint8_t nr_threads = 4;
    uint64_t sample_size = 0x4000000;
    uint64_t factor = sample_size / nr_threads;

    // Test settings
    const uint32_t IPM[4] = {
        0x01000000,
        0x00000000,
        0x00000000,
        0x00000000};
    const uint32_t OPM[4] = {
        0x5CF46CE4,
        0x9273D3B1,
        0xADF95750,
        0x702F4F6F};
    // Coefficients corresponding with 0x1 as output mask
    const int8_t COEFFS[256] = {
        0, 12, 2, 6, -8, 8, 6, -10, 2, -10, -4, 0, -6, -14, 8, 8, -12, 0, -10,
        2, 12, 4, 2, 2, 14, -14, -8, -12, 6, -10, -4, -4, 6, 2, 8, 12, -2, -2,
        -4, -4, 8, -12, 2, -2, 0, -16, -2, 6, -6, -2, -4, 0, 2, 2, 8, 0, -4,
        0, 6, -14, -12, -12, -6, 10, 4, -8, 6, 2, 12, -12, 2, 10, -10, -6, 8,
        12, -10, -2, 4, -12, -12, 8, 6, 10, -4, -4, -6, 2, 6, -6, -8, -12, 6,
        -10, -4, -4, -6, 6, 12, 0, -6, -6, 0, -16, -12, 8, -10, -6, 12, 4, 10,
        10, -14, 6, 4, -8, 2, -14, 0, -8, -12, 0, 6, -6, 12, 4, 2, -6, -12, 8,
        6, 2, -4, 12, -14, -14, -14, 6, 4, 8, 2, 2, -16, -8, -8, -12, 2, 14,
        -8, 8, -2, 6, -2, 10, -8, -4, 6, -10, 4, 4, 2, -2, -4, -16, -6, 2, 8,
        0, -8, -4, 2, -10, 8, 8, -2, -10, 6, 2, -8, 4, 6, -10, 4, -4, 12, 0,
        -2, -6, -12, 12, -6, 2, -8, -12, 10, 14, -16, -8, 14, -10, 14, 2, 8,
        12, 6, 6, 4, -4, 0, 4, 10, -2, 0, -8, -2, -2, -10, 2, -8, -4, -10, -10,
        -12, -12, 6, 2, 0, 4, 6, -2, 12, -12, -4, 8, 14, -6, 12, -12, 2, 2, 6,
        2, 8, 4, -2, 14, -12, 12, 4, 0, 14, 2, -4, 12, 2, 2};

    // Linearization offset constant
    const int16_t sum = std::accumulate(COEFFS, COEFFS + 256, 0);
    const uint16_t offset = (1 - (sum >> 7)) >> 1;

    uint64_t counts[nr_threads];

    // Initialize plaintexts
    uint32_t pt_[4 * nr_threads];
    for (uint16_t i = 0; i < 4 * nr_threads; i++)
    {
        pt_[i] = rand_uint32();
    }

    // Count
    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for (uint16_t t = 0; t < nr_threads; t++)
    {
        counts[t] = 0;

        // Variables
        int32_t linearized;
        uint16_t sect, in_parity, out_parity;
        uint32_t pt[4], *_pt = &pt_[4 * t];

        // Count
        for (uint64_t i = 0; i < factor; i++)
        {
            // Create plaintext
            _pt[0] += 983;
            _pt[1] += 939391;
            _pt[2] += 39916801;
            _pt[3] += 1301476963;

            pt[0] = _pt[0];
            pt[1] = _pt[1];
            pt[2] = _pt[2];
            pt[3] = _pt[3];

            // Analyse plaintext for 2 rounds
            linearized = 0;
            sect = pt[0] >> 24;
            for (uint16_t ipm = 0; ipm < 0x100; ipm++)
            {
                linearized += COEFFS[ipm] * P32(sect & ipm);
            }
            in_parity = (offset + (linearized >> 7));

            // Encrypt
            for (uint8_t i = 0; i < 2; i++)
            {
                SubBytes(pt);
                ShiftRows(pt);
                MixColumns(pt);
            }
            out_parity = P32(pt[0] & OPM[0] ^ pt[1] & OPM[1] ^ pt[2] & OPM[2] ^ pt[3] & OPM[3]);

            counts[t] += in_parity ^ out_parity;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration (mus): " << duration.count() << '\n' << '\n';

    printResults(counts, nr_threads, sample_size);
}

int main()
{
    trailTwoRounds();
    return 0;
}