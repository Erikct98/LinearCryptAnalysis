#include "toolbox.h"
#include <iostream>

void trailMixColumn()
{
    uint32_t IPM = 0x01808101;
    uint32_t OPM = 0x00010000;
    uint32_t ct;

    uint64_t count = 0;
    for (uint64_t pt = 0; pt < 0x100000000; pt++)
    {
        ct = MixColumn(pt);
        count += P32(pt & IPM ^ ct & OPM);
    }
    std::cout << count << std::endl;
}

int main()
{
    trailMixColumn();
    return 0;
}