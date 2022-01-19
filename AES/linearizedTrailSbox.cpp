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
    return (0x6996 >> (v ^ v >> 4) & 0xF) & 0x1;
}

void linearize_sbox_with_5_equations()
{
    uint8_t ct;
    uint16_t count = 0;
    uint8_t in_parity, out_parity;
    int linearized, linearized2;
    int a1, a2, a3, a4, a5;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        // Encrypt
        ct = SubByte(pt);

        a1 = I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 7));
        a3 = I(F(pt, 1) ^ F(pt, 4));
        a4 = I(F(pt, 1) ^ F(pt, 7));
        a5 = I(F(pt, 2) ^ F(pt, 4));

        linearized2 = I(F(pt, 1) ^ F(pt, 2)) + I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 7)) + I(F(pt, 1) ^ F(pt, 4)) + I(F(pt, 1) ^ F(pt, 7)) + I(F(pt, 2) ^ F(pt, 4));

        // Linearize
        linearized = a1 + a3 + a4 + a5;
        std::cout << a1 << " " << a2 << " " << a3 << " " << a4 << " " << a5 << " " << linearized << " " << linearized2 << std::endl;

        in_parity = linearized >= 0;
        out_parity = getParity(ct & OPM);

        count += (in_parity ^ out_parity) & 1;
    }
    std::cout << count << "/" << 256 << std::endl;
}

void linearize_sbox_with_21_equations()
{
    uint8_t ct;
    uint16_t count = 0;
    uint8_t in_parity, out_parity;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        // Encrypt
        ct = SubByte(pt);

        // Linearize
        linearized = I(F(pt, 0) ^ F(pt, 2)) \
                    + I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2)) \
                    - I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3)) \
                    + I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3) ^ F(pt, 4)) \
                    - I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3) ^ F(pt, 5)) \
                    - I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 5)) \
                    + I(F(pt, 0) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 5)) \
                    - I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 5)) \
                    + I(F(pt, 1) ^ F(pt, 2)) \
                    - I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 6) ^ F(pt, 7)) \
                    + I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 6)) \
                    + I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 7)) \
                    - I(F(pt, 1) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6)) \
                    + I(F(pt, 1) ^ F(pt, 4)) \
                    + I(F(pt, 1) ^ F(pt, 5) ^ F(pt, 6)) \
                    + I(F(pt, 1) ^ F(pt, 7)) \
                    - I(F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6)) \
                    + I(F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7)) \
                    + I(F(pt, 2) ^ F(pt, 4)) \
                    - I(F(pt, 2) ^ F(pt, 6) ^ F(pt, 7)) \
                    + I(F(pt, 4) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7));

        in_parity = linearized >= 0;
        out_parity = getParity(ct & OPM);

        count += in_parity ^ out_parity;
    }
    std::cout << count << "/" << 256 << std::endl;
}

int main()
{
    linearize_sbox_with_5_equations();
    return 0;
}