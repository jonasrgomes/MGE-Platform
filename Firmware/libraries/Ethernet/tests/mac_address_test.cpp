#include <cassert>
#include <cstring>
#include <iostream>

#include "mge/ethernet/mac_address.h"

using mge::ethernet::MacAddress;

int main()
{
    {
        const MacAddress empty;
        assert(empty.isZero());
        assert(!empty.isBroadcast());
        assert(!empty.isUnicast());
    }

    {
        const MacAddress mac(0x02, 0x12, 0x34, 0x56, 0x78, 0x01);
        assert(!mac.isZero());
        assert(!mac.isBroadcast());
        assert(!mac.isMulticast());
        assert(mac.isUnicast());

        char text[18] = {};
        assert(mac.toString(text, sizeof(text)));
        assert(std::strcmp(text, "02:12:34:56:78:01") == 0);
    }

    {
        const MacAddress goose(0x01, 0x0C, 0xCD, 0x01, 0x00, 0x01);
        assert(goose.isMulticast());
        assert(!goose.isUnicast());
    }

    {
        const MacAddress broadcast = MacAddress::broadcast();
        assert(broadcast.isBroadcast());
        assert(broadcast.isMulticast());
    }

    {
        const std::uint8_t bytes[MacAddress::Size] = {
            0x10, 0x20, 0x30, 0x40, 0x50, 0x60
        };

        const MacAddress a(bytes);
        const MacAddress b(0x10, 0x20, 0x30, 0x40, 0x50, 0x60);
        const MacAddress c(0x10, 0x20, 0x30, 0x40, 0x50, 0x61);

        assert(a == b);
        assert(a != c);
        assert(a[0] == 0x10);
        assert(a[5] == 0x60);
        assert(a[99] == 0);
    }

    std::cout << "All MacAddress tests passed.\n";
    return 0;
}
