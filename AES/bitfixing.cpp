#include "toolbox.h"
#include "aes.h"
#include <numeric>

#define ROUNDS 3

void bitfixing()
{
    // Settings
    const uint32_t sample_size = 0x80000000;
    uint32_t key[4] = {
        0xC4298E25,
        0x2DB74D07,
        0xF85B2ABC,
        0xD04C1C6F};
    uint32_t expandedkey[4 * (ROUNDS + 1)];
    ExpandKey(key, expandedkey, 3);

    const uint32_t IPM = 0x440A1313;
    const uint32_t OPM = 0x481B0C38;

    // const uint32_t GREY = 0x692C94C5, GREY2 = 0xA54C3C3E;
    const uint32_t GREY = 0x4910C704, GREY2 = 0xD9144011;

    // Setup local variables
    uint64_t count = 0;
    uint32_t pt[4], ppt[4];
    uint32_t ipp, opp;

    // Randomly init random plaintext
    for (uint16_t j = 0; j < 4; j++)
    {
        ppt[j] = rand_uint32();
    }

    for (uint32_t i = 0; i < sample_size; i++)
    {
        // Choose random plaintext
        for (uint16_t j = 0; j < 4; j++)
        {
            ppt[j] += (j + 1) * 0x1924216891;
        }

        // Fix grey parts
        pt[0] = ppt[0] & 0x00FFFFFF ^ GREY & 0xFF000000; // ^ GREY2 & 0x000000FF;
        pt[1] = ppt[1] & 0xFF00FFFF ^ GREY & 0x00FF0000; // ^ GREY2 & 0xFF000000;
        pt[2] = ppt[2] & 0xFFFF00FF ^ GREY & 0x0000FF00; // ^ GREY2 & 0x00FF0000;
        pt[3] = ppt[3] & 0xFFFFFF00 ^ GREY & 0x000000FF; // ^ GREY2 & 0x0000FF00;

        // Determine input parity
        ipp = P32((pt[0] & 0xFF000000 ^
                    pt[1] & 0x00FF0000 ^
                    pt[2] & 0x0000FF00 ^
                    pt[3] & 0x000000FF) &
                    IPM);

        // Encrypt
        encrypt(pt, expandedkey, ROUNDS);

        // Determine output parity
        opp = P32((pt[0] & 0xFF000000 ^
                    pt[3] & 0x00FF0000 ^
                    pt[2] & 0x0000FF00 ^
                    pt[1] & 0x000000FF) &
                    OPM);

        count += ipp ^ opp;
    }

    // Print results
    printResults(&count, 1, sample_size);
}

void bitfixcolbinning()
{
    // Settings
    const uint32_t sample_size = 0x100;
    uint32_t pt, out;
    uint64_t counter = 0;
    uint32_t IPM = 0x10000000;
    uint32_t OPM = 0x481B0C38;
    uint32_t grey = 0xBA000000;

    uint32_t bins[32];
    for (uint32_t i = 0; i < 32; i++)
    {
        bins[i] = 0;
    }

    uint64_t sum = 0;
    for (uint32_t pt = grey; pt < grey + sample_size; pt++)
    {
        // Determine input parity
        out = SubByteCol(MixColumn(pt));
        std::cout << std::hex << pt << " " << out << std::endl;
        for (uint32_t i = 0; i < 32; i++)
        {
            bins[i] += out & 1;
            out = out >> 1;
        }
    }

    std::cout << sum << std::endl;

    //
    for (uint32_t i = 0; i < 32; i++)
    {
        std::cout << i << ": " << bins[i] << std::endl;
    }
}

int main()
{
    bitfixing();
    // bitfixcolbinning();
    return 0;
}