#include <iostream>
#include "chrono"

// CYclic LEft SHift uint32_t's by X bits
#define sw8cylesh32_1(word) (((word & 0x7F7F7F7F) << 1 ^ (word & 0x80808080) >> 7))
#define cylesh32_8(word) (word << 8 ^ word >> 24)
#define cylesh32_16(word) (word << 16 ^ word >> 16)
#define cylesh32_24(word) (word << 24 ^ word >> 8)

// Lookup table for sbox
static const uint8_t sbox[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
    0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
    0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
    0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
    0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
    0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
    0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
    0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
    0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
    0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
    0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
    0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

static const int8_t coeffs[256] = {
    0, 12, 2, 6, -8, 8, 6, -10, 2, -10, -4, 0, -6, -14, 8, 8, -12, 0, -10, 2, 12, 4, 2, 2, 14, -14, -8, -12, 6, -10, -4, -4, 6, 2, 8, 12, -2, -2, -4, -4, 8, -12, 2, -2, 0, -16, -2, 6, -6, -2, -4, 0, 2, 2, 8, 0, -4, 0, 6, -14, -12, -12, -6, 10, 4, -8, 6, 2, 12, -12, 2, 10, -10, -6, 8, 12, -10, -2, 4, -12, -12, 8, 6, 10, -4, -4, -6, 2, 6, -6, -8, -12, 6, -10, -4, -4, -6, 6, 12, 0, -6, -6, 0, -16, -12, 8, -10, -6, 12, 4, 10, 10, -14, 6, 4, -8, 2, -14, 0, -8, -12, 0, 6, -6, 12, 4, 2, -6, -12, 8, 6, 2, -4, 12, -14, -14, -14, 6, 4, 8, 2, 2, -16, -8, -8, -12, 2, 14, -8, 8, -2, 6, -2, 10, -8, -4, 6, -10, 4, 4, 2, -2, -4, -16, -6, 2, 8, 0, -8, -4, 2, -10, 8, 8, -2, -10, 6, 2, -8, 4, 6, -10, 4, -4, 12, 0, -2, -6, -12, 12, -6, 2, -8, -12, 10, 14, -16, -8, 14, -10, 14, 2, 8, 12, 6, 6, 4, -4, 0, 4, 10, -2, 0, -8, -2, -2, -10, 2, -8, -4, -10, -10, -12, -12, 6, 2, 0, 4, 6, -2, 12, -12, -4, 8, 14, -6, 12, -12, 2, 2, 6, 2, 8, 4, -2, 14, -12, 12, 4, 0, 14, 2, -4, 12, 2, 2};

inline uint8_t SubByte(uint8_t word)
{
    return sbox[word & 0xFF];
}

inline uint32_t SubBytes(uint32_t col)
{
    uint8_t *tmp = (uint8_t *)&col;
    for (uint8_t i = 0; i < 4; i++)
    {
        tmp[i] = SubByte(tmp[i]);
    }
    return col;
}

inline uint32_t getParity(uint32_t v)
{
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v &= 0xF;
    return (0x6996 >> v) & 0x1;
}

inline uint32_t mulp2(uint32_t a)
{
    uint32_t x = sw8cylesh32_1(a);
    return (x & 0xFEFEFEFE) ^ ((x & 0x01010101) * 0x1B);
}

inline uint32_t MixColumn(uint32_t state)
{
    uint32_t tmp;
    tmp = mulp2(state);
    tmp ^= cylesh32_8(tmp) ^ cylesh32_8(state);
    tmp ^= cylesh32_16(state);
    tmp ^= cylesh32_24(state);
    return tmp;
}

inline int8_t I(uint8_t x)
{
    return 1 - (x << 1);
}

inline uint8_t J(int8_t x)
{
    return (1 - x) >> 1;
}

void trailOneRound()
{
    uint32_t IPM = 0x01000000; // mask after sbox
    uint32_t OPM = 0xE0A16121;
    uint32_t ct, xt;
    uint8_t sect;

    int16_t sum = 0;
    for (uint16_t j = 0; j < 0x100; j++)
    {
        sum += coeffs[j];
    }
    std::cout << sum << std::endl;
    uint16_t term = (1 - (sum >> 7)) >> 1;
    std::cout << term << std::endl;

    int32_t linearized;
    uint64_t counts[4];
    for (uint16_t j = 0; j < 4; j++)
    {
        counts[j] = 0;
    }

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t factor = 0x40000000;
    #pragma omp parallel for
    for (uint16_t j = 0; j < 4; j++)
    {
        for (uint64_t pt = j * factor; pt < (j + 1) * factor; pt += 0x1)
        {
            ct = MixColumn(SubBytes(pt));

            linearized = 0;
            sect = pt >> 24;
            for (uint16_t ipm = 0; ipm < 0x100; ipm++)
            {
                linearized += coeffs[ipm] * getParity(ipm & sect);
            }
            counts[j] += getParity(ct & OPM) ^ (term + (linearized >> 7));
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    // Print results to console
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration (mus): " << duration.count() << '\n';

    std::cout << counts[0] << " " <<
                 counts[1] << " " <<
                 counts[2] << " " <<
                 counts[3] << std::endl;

    // pt = 0x0B0E090D;
    // std::cout << std::hex << pt << " " << ct << std::endl;
}

int main()
{
    trailOneRound();
    return 0;
}