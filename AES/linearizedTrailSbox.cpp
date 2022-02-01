#include "toolbox.h"

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

bool approx_74_with_5(uint8_t word)
{
    return (I(F(word, 1) ^ F(word, 2))\
         + I(F(word, 1) ^ F(word, 2) ^ F(word, 4) ^ F(word, 7))\
         + I(F(word, 1) ^ F(word, 4))\
         + I(F(word, 1) ^ F(word, 7))\
         + I(F(word, 2) ^ F(word, 4)) ) >= 0;
}

bool approx_7A_with_1(uint8_t pt)
{
    return (F(pt, 3) | F(pt, 7)) ^ (F(pt, 5) | F(pt, 1));
}

bool approx_7A_with_5(uint8_t pt)
{
    return (  I(F(pt, 2) ^ F(pt, 6))\
            - I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4))\
            - I(F(pt, 0) ^ F(pt, 1))\
            + I(F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6))\
            - I(F(pt, 1) ^ F(pt, 5) ^ F(pt, 7))) >= 0;
}

uint8_t approx_7A_with_10(uint8_t pt)
{
    return (-16 * I(F(pt, 3) ^ F(pt, 5) ^ F(pt, 6))
            +-16 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 6) ^ F(pt, 7))
            +-14 * I(F(pt, 0) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7))
            +-16 * I(F(pt, 3) ^ F(pt, 5) ^ F(pt, 7))
            +-16 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3) ^ F(pt, 5) ^ F(pt, 7))
            + 14 * I(F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6))
            +-14 * I(F(pt, 1) ^ F(pt, 5) ^ F(pt, 7))
            + 14 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 5))
            +-16 * I(F(pt, 0) ^ F(pt, 1))
            + 14 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 7))) >= 0;
}

uint8_t approx_7A_with_20(uint8_t pt)
{
    return ( 14 * I(F(pt, 1) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7))
           +-12 * I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 7))
           + 14 * I(F(pt, 3))
           + 14 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 7))
           +-16 * I(F(pt, 3) ^ F(pt, 5) ^ F(pt, 6))
           +-12 * I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6) ^ F(pt, 7))
           + 12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4))
           +-14 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2))
           +-12 * I(F(pt, 1) ^ F(pt, 4) ^ F(pt, 6))
           + 14 * I(F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6))
           +-16 * I(F(pt, 0) ^ F(pt, 1))
           +-12 * I(F(pt, 2) ^ F(pt, 4) ^ F(pt, 5))
           + 14 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 5))
           +-16 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3) ^ F(pt, 5) ^ F(pt, 7))
           +-12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6))
           + 12 * I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 5) ^ F(pt, 6))
           + 12 * I(F(pt, 0) ^ F(pt, 2) ^ F(pt, 4))
           +-14 * I(F(pt, 1) ^ F(pt, 3) ^ F(pt, 5))
           +-12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 5))
           + 12 * I(F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 7))) >= 0;
}

uint8_t approx_74_with_21(uint8_t word)
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

uint8_t approx_7A_with_50(uint8_t pt)
{
    return ( 12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4))
          + -12 * I(F(pt, 0) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 7))
          + -16 * I(F(pt, 3) ^ F(pt, 5) ^ F(pt, 7))
          + -12 * I(F(pt, 1) ^ F(pt, 6) ^ F(pt, 7))
          +  12 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 6) ^ F(pt, 7))
          + -12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3))
          +  14 * I(F(pt, 1) ^ F(pt, 2))
          +  12 * I(F(pt, 1) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6))
          +  12 * I(F(pt, 0))
          +  14 * I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 7))
          + -12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 5))
          +  12 * I(F(pt, 2) ^ F(pt, 6))
          + -12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6))
          + -16 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 6) ^ F(pt, 7))
          +  14 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 7))
          + -14 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4))
          + -12 * I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6) ^ F(pt, 7))
          + -12 * I(F(pt, 1) ^ F(pt, 3) ^ F(pt, 7))
          +  14 * I(F(pt, 3))
          +  12 * I(F(pt, 3) ^ F(pt, 4) ^ F(pt, 6) ^ F(pt, 7))
          +  14 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5))
          +  14 * I(F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6))
          +  12 * I(F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 7))
          +  12 * I(F(pt, 3) ^ F(pt, 7))
          + -12 * I(F(pt, 0) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6))
          + -16 * I(F(pt, 3) ^ F(pt, 5) ^ F(pt, 6))
          + -12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 7))
          + -16 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3) ^ F(pt, 5) ^ F(pt, 7))
          + -12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 7))
          +  12 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7))
          + -14 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 2))
          + -14 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 7))
          + -12 * I(F(pt, 0) ^ F(pt, 2) ^ F(pt, 3) ^ F(pt, 6) ^ F(pt, 7))
          +  12 * I(F(pt, 3) ^ F(pt, 4))
          + -14 * I(F(pt, 1) ^ F(pt, 5) ^ F(pt, 7))
          + -12 * I(F(pt, 0) ^ F(pt, 4) ^ F(pt, 7))
          +  12 * I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 5) ^ F(pt, 6))
          +  12 * I(F(pt, 0) ^ F(pt, 2) ^ F(pt, 4))
          + -14 * I(F(pt, 0) ^ F(pt, 1) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 7))
          +  12 * I(F(pt, 2) ^ F(pt, 7))
          + -12 * I(F(pt, 1) ^ F(pt, 3) ^ F(pt, 5) ^ F(pt, 7))
          + -12 * I(F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 6))
          + -16 * I(F(pt, 0) ^ F(pt, 1))
          + -12 * I(F(pt, 1) ^ F(pt, 2) ^ F(pt, 4) ^ F(pt, 5))
          + -12 * I(F(pt, 2) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 7))
          + -12 * I(F(pt, 2) ^ F(pt, 4) ^ F(pt, 5))
          +  12 * I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7))
          + -12 * I(F(pt, 0) ^ F(pt, 3) ^ F(pt, 7))
          +  14 * I(F(pt, 1) ^ F(pt, 4) ^ F(pt, 5) ^ F(pt, 6) ^ F(pt, 7))
          + -14 * I(F(pt, 1) ^ F(pt, 3) ^ F(pt, 5))) >= 0;
}

void linearize_sbox_output_74_with_5_terms()
{
    uint8_t OPM = 0x74;
    uint8_t pt, ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByteByte(pt);
        count += (approx_74_with_5(pt) ^ P8(ct & OPM)) & 1;
    }
    std::cout << "--- 74 - 5 TERMS ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}

void linearize_sbox_output_7A_with_1_term()
{
    uint8_t OPM = 0x7A;
    uint8_t pt, ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByteByte(pt);
        count += (approx_7A_with_1(pt) ^ P8(ct & OPM)) & 1;
    }
    std::cout << "--- 7A - 1 TERM ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}

void linearize_sbox_output_7A_with_5_terms()
{
    uint8_t OPM = 0x7A;
    uint8_t pt, ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByteByte(pt);
        count += (approx_7A_with_5(pt) ^ P8(ct & OPM)) & 1;
    }
    std::cout << "--- 7A - 5 TERMS ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}

void linearize_sbox_output_7A_with_10_terms()
{
    uint8_t OPM = 0x7A;
    uint8_t pt, ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByteByte(pt);
        count += (approx_7A_with_10(pt) ^ P8(ct & OPM)) & 1;
    }
    std::cout << "--- 7A - 10 TERMS ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}


void linearize_sbox_output_7A_with_20_terms()
{
    uint8_t OPM = 0x7A;
    uint8_t pt, ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByteByte(pt);
        count += (approx_7A_with_20(pt) ^ P8(ct & OPM)) & 1;
    }
    std::cout << "--- 7A - 20 TERMS ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}


void linearize_sbox_output_74_with_21_terms()
{
    uint8_t OPM = 0x74;
    uint8_t ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByteByte(pt);
        count += (approx_74_with_21(pt) ^ P8(ct & OPM)) & 1;
    }
    std::cout << "--- 74 - 21 TERMS ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}

void linearize_sbox_output_7A_with_50_terms()
{
    uint8_t OPM = 0x7A;
    uint8_t ct;
    uint16_t count = 0;
    int linearized;
    for (uint16_t pt = 0; pt < 0x100; pt++)
    {
        ct = SubByteByte(pt);
        count += (approx_7A_with_50(pt) ^ P8(ct & OPM)) & 1;
    }
    std::cout << "--- 7A - 50 TERMS ---" << std::endl;
    std::cout << "count: " << count << " / " << 256 << std::endl;
    std::cout << "corr: " << (2 * (count / 256.0) - 1) << std::endl;
}

int main()
{
    linearize_sbox_output_7A_with_1_term();
    linearize_sbox_output_7A_with_5_terms();
    linearize_sbox_output_7A_with_10_terms();
    linearize_sbox_output_7A_with_20_terms();
    linearize_sbox_output_7A_with_50_terms();
    linearize_sbox_output_74_with_5_terms();
    linearize_sbox_output_74_with_21_terms();
    return 0;
}