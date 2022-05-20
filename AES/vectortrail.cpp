#include "toolbox.h"
#include <bitset>

// 2 round trail, active sboxes: 4 - 1, diagonal to top left.
void vectortrail()
{
    const uint8_t ROUNDS = 2;

    const uint8_t imm = 0x01;
    const uint8_t ipms[12] = {0xA3, 0x2D, 0xC4, 0xA3, 0x2D, 0xC4, 0x9F, 0xAB, 0x68, 0x81, 0xB8, 0x6E};
    const uint8_t opms[3] = {0xC0, 0x48, 0x50};

    // const uint8_t imm = 0x02;
    // const uint8_t ipms[12] = {0xFD, 0xB0, 0x6A, 0xFD, 0xB0, 0x6A, 0x0D, 0x21, 0xCA, 0x9F, 0xAB, 0x68};
    // const uint8_t opms[3] = {0x40, 0x38, 0xCF};

    // const uint8_t imm = 0x03;
    // const uint8_t ipms[12] = {0x4F, 0x9A, 0xFB, 0x4F, 0x9A, 0xFB, 0xFD, 0xB0, 0x6A, 0xA3, 0x2D, 0xC4};
    // const uint8_t opms[3] = {0x60, 0x1A, 0x24};

    uint64_t ct;
    uint8_t *ptr;
    uint16_t mask;

    // Vector counters
    const uint16_t veclen = 15;
    uint32_t cnt[1 << veclen];
    std::fill(cnt, cnt + (1 << veclen), 0);

    // Key
    uint32_t key[4] = {
        0xC652264D,
        0x75E409C8,
        0x5CBF89DC,
        0x4A83627A};
    uint32_t expandedKey[4 * (ROUNDS + 1)];
    ExpandKey(key, expandedKey, ROUNDS + 1);
    key[0] = (expandedKey[0] & 0xFF000000) ^ (expandedKey[1] & 0xFF0000) ^ (expandedKey[2] & 0xFF00) ^ (expandedKey[3] & 0xFF);
    key[1] = expandedKey[4];
    key[2] = expandedKey[8];

    for (uint64_t pt = 0; pt < 0x100000000; pt++)
    // for (uint64_t pt = 0; pt < 0x3000; pt++)
    {
        // Analyse plaintext
        mask = 0;
        ptr = (uint8_t *)&pt;
        for (uint16_t i = 0; i < 4; i++)
        {
            for (uint16_t j = 0; j < 3; j++)
            {
                mask <<= 1;
                mask ^= P8(ipms[(3 * i) + j] & ptr[i]);
            }
        }

        // Encrypt 2 rounds
        // Including key addition
        ct = pt ^ key[0];
        ct = SubByteCol(ct);
        ct = MixColumn(ct);
        ct = ct ^ key[1];
        ct = SubByteCol(ct);
        ct = ct ^ key[2];

        // Analyse ciphertext
        ptr = (uint8_t *)&ct;
        for (uint16_t j = 0; j < 3; j++)
        {
            mask <<= 1;
            mask ^= P8(opms[j] & ptr[3]);
        }

        // Track counts
        cnt[mask] += 1;
    }

    for (uint32_t i = 0; i < (1 << veclen); i++)
    {
        std::cout << std::bitset<15>(i) << ": " << cnt[i] << std::endl;
    }
}

// void lineartrail()
// {
//     const uint32_t key[3] = {
//         0x44377A68,
//         0x38A64392,
//         0xC690F2AE,
//     };
//     const uint8_t ipms[4] = {0x2D, 0x2D, 0xAB, 0x81};
//     const uint8_t opm = 0x48;

//     uint8_t ipp, opp;
//     uint64_t ct;
//     uint8_t *ptr;

//     // Vector counters
//     uint64_t cnt = 0;

//     for (uint64_t pt = 0; pt < 0x100000000; pt++)
//     // for (uint64_t pt = 0; pt < 0x3000; pt++)
//     {
//         // Analyse plaintext
//         ipp = 0;
//         ptr = (uint8_t *) &pt;
//         for (uint16_t i = 0; i < 4; i++)
//         {
//             ipp ^= P8(ipms[i] & ptr[i]);
//         }

//         // Encrypt 2 rounds
//         ct = pt ^ key[0];
//         ct = SubByteCol(ct);
//         ct = MixColumn(ct);
//         ct = pt ^ key[1];
//         ct = SubByteCol(ct);
//         ct = pt ^ key[2];

//         // Analyse ciphertext
//         ptr = (uint8_t *) &ct;
//         opp = P8(opm & ptr[3]);

//         // Track counts
//         cnt += ipp ^ opp;
//     }

//     std::cout << cnt << std::endl;
// }

int main()
{
    vectortrail();
    // lineartrail();
}