#include <iostream>

// CYclic LEft SHift uint32_t's by X bits
#define sw8cylesh32_1(word) (((word & 0x7F7F7F7F) << 1 ^ (word & 0x80808080) >> 7))
#define cylesh32_8(word) (word << 8 ^ word >> 24)
#define cylesh32_16(word) (word << 16 ^ word >> 16)
#define cylesh32_24(word) (word << 24 ^ word >> 8)

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

uint32_t MixColumn(uint32_t state)
{
    uint32_t tmp;
    tmp = mulp2(state);
    tmp ^= cylesh32_8(tmp) ^ cylesh32_8(state);
    tmp ^= cylesh32_16(state);
    tmp ^= cylesh32_24(state);
    return tmp;
}


void trailMixColumn()
{
    uint32_t IPM = 0x01808101;
    uint32_t OPM = 0x00010000;
    uint32_t ct;

    uint64_t count = 0;
    for (uint64_t pt = 0; pt < 0x100000000; pt++)
    {
        ct = MixColumn(pt);
        count += getParity(pt & IPM ^ ct & OPM);
    }
    std::cout << count << std::endl;

    // pt = 0x0B0E090D;
    // std::cout << std::hex << pt << " " << ct << std::endl;
}


int main()
{
    trailMixColumn();
    return 0;
}