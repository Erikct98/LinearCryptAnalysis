#include "aes.h"

#define OPM 0x74

int I(uint8_t x)
{
    return 1 - (x << 1);
}

uint8_t J(int x)
{
 return (1 - x) >> 1;
}

uint8_t F(uint8_t x, uint8_t i)
{
    return x >> i & 0x1;
}

inline uint8_t getParity(uint8_t v)
{
    v ^= v >> 4;
    v &= 0xf;
    return (0x6996 >> v) & 0x1;
}

uint8_t approx_7A_with_5(uint8_t word)
{
    return (I(F(word, 1) ^ F(word, 2))\
         + I(F(word, 1) ^ F(word, 2) ^ F(word, 4) ^ F(word, 7))\
         + I(F(word, 1) ^ F(word, 4))\
         + I(F(word, 1) ^ F(word, 7))\
         + I(F(word, 2) ^ F(word, 4)) ) >= 0;
}

uint8_t approx_7A_with_21(uint8_t word)
{
    return (I(F(word, 0) ^ F(word, 2)) \
         + I(F(word, 0) ^ F(word, 1) ^ F(word, 2)) \
         - I(F(word, 0) ^ F(word, 1) ^ F(word, 3)) \
         + I(F(word, 0) ^ F(word, 1) ^ F(word, 3) ^ F(word, 4)) \
         - I(F(word, 0) ^ F(word, 1) ^ F(word, 3) ^ F(word, 5)) \
         - I(F(word, 0) ^ F(word, 1) ^ F(word, 2) ^ F(word, 3) ^ F(word, 5)) \
         + I(F(word, 0) ^ F(word, 2) ^ F(word, 4) ^ F(word, 5)) \
         - I(F(word, 0) ^ F(word, 1) ^ F(word, 2) ^ F(word, 4) ^ F(word, 5)) \
         + I(F(word, 1) ^ F(word, 2)) \
         - I(F(word, 1) ^ F(word, 2) ^ F(word, 3) ^ F(word, 6) ^ F(word, 7)) \
         + I(F(word, 1) ^ F(word, 2) ^ F(word, 4) ^ F(word, 6)) \
         + I(F(word, 1) ^ F(word, 2) ^ F(word, 4) ^ F(word, 7)) \
         - I(F(word, 1) ^ F(word, 3) ^ F(word, 4) ^ F(word, 6)) \
         + I(F(word, 1) ^ F(word, 4)) \
         + I(F(word, 1) ^ F(word, 5) ^ F(word, 6)) \
         + I(F(word, 1) ^ F(word, 7)) \
         - I(F(word, 2) ^ F(word, 3) ^ F(word, 4) ^ F(word, 5) ^ F(word, 6)) \
         + I(F(word, 2) ^ F(word, 3) ^ F(word, 4) ^ F(word, 5) ^ F(word, 6) ^ F(word, 7)) \
         + I(F(word, 2) ^ F(word, 4)) \
         - I(F(word, 2) ^ F(word, 6) ^ F(word, 7)) \
         + I(F(word, 4) ^ F(word, 5) ^ F(word, 6) ^ F(word, 7))) >= 0;
}

void linearize_sbox_output_with_5_terms()
{
    uint8_t pt, ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByte(pt);
        count += (approx_7A_with_5(pt) ^ getParity(ct & OPM)) & 1;
    }
    std::cout << "--- 5 TERMS ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}

void linearize_sbox_output_with_21_terms()
{
    uint8_t ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByte(pt);
        count += (approx_7A_with_21(pt) ^ getParity(ct & OPM)) & 1;
    }
    std::cout << "--- 21 TERMS ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}

int main()
{
    linearize_sbox_output_with_5_terms();
    linearize_sbox_output_with_21_terms();
    return 0;
}