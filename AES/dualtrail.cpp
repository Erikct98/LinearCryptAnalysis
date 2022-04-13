#include "toolbox.h"
#include <random>
#include <algorithm>

// Trail two sboxes simultaneously with wrong keys.

void dualtrail()
{
    uint32_t sampleSize = 0x10000;
    uint32_t nrTrails = 0x1000;
    uint8_t opm = 0x2D;
    uint16_t opmopm = 0x2D2D;
    uint8_t ipms[3] = {0x93, 0x1A, 0x3A};
    uint16_t ipp, opp, p0, p1, p2;
    uint32_t pt, ct, key;


    uint32_t bestkey = 0;
    double bestcorr = -11;
    for (uint32_t t = 0; t < nrTrails; t++)
    {

        key = t;
        // rand_uint32() & 0xFFFF;
        // std::cout << "key: 0x" << std::hex << key << std::dec << ",\n";

        // std::cout << "category: "
        //           << (uint16_t) P8((key >> 8 & ipms[0])) << ""
        //           << (uint16_t) P8((key >> 8 & ipms[1])) << ""
        //           << (uint16_t) P8((key >> 8 & ipms[2])) << ""
        //           << (uint16_t) P8((key & ipms[0])) << ""
        //           << (uint16_t) P8((key & ipms[1])) << ""
        //           << (uint16_t) P8((key & ipms[2])) << ", ";

        uint32_t counts = sampleSize;
        for (uint32_t i = 0; i < sampleSize; i++)
        {
            pt = i;
            p0 = P8(pt & ipms[0]);
            p1 = P8(pt & ipms[1]);
            p2 = P8(pt & ipms[2]);
            ipp = (p0 + p1 + p2 + (p0 ^ p1) + (p0 ^ p2)) >= 3;
            // std::cout << "p0: " << p0 << ", "
            //           << "p1: " << p1 << ", "
            //           << "p2: " << p2 << ", "
            //           << "ipp: " << ipp << "\n";

            p0 = P8((pt >> 8) & ipms[0]);
            p1 = P8((pt >> 8) & ipms[1]);
            p2 = P8((pt >> 8) & ipms[2]);
            ipp ^= (p0 + p1 + p2 + (p0 ^ p1) + (p0 ^ p2)) >= 3;

            // std::cout << p0 << " "
            //           << p1 << " "
            //           << p2 << " "
            //           << ipp << "\n";

            // Encrypt
            // std::cout << std::hex << pt << std::dec << "\n";
            pt ^= key;
            // std::cout << std::hex << pt << std::dec << "\n";
            ct = SubByteCol(pt) & 0xFFFF;
            // std::cout << std::hex << ct << std::dec << "\n";

            // Analyse ciphertext
            opp = P32(ct & opmopm);
            // std::cout << std::hex
            //           << opp << " "
            //           << opmopm
            //           << std::dec <<"\n";

            // Update counts
            counts -= ipp ^ opp;
        }
        float corr = 2 * counts / (float)sampleSize - 1;
        float correxp = std::log2(std::abs(corr));

        if (correxp > bestcorr)
        {
            bestcorr = correxp;
            bestkey = key;
        }
    }
    std::cout << "key: " << std::hex << bestkey << ", correxp: " << std::dec << bestcorr << std::endl;
}

int main()
{
    dualtrail();
    return 0;
}