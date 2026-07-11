#include <iostream>

#include "mge/ethernet/mac_address.h"

int main()
{
    const mge::ethernet::MacAddress gooseDestination(
        0x01, 0x0C, 0xCD, 0x01, 0x00, 0x01);

    char text[18] = {};

    if (gooseDestination.toString(text, sizeof(text)))
    {
        std::cout << "GOOSE destination MAC: " << text << '\n';
    }

    std::cout
        << "Multicast: "
        << (gooseDestination.isMulticast() ? "yes" : "no")
        << '\n';

    return 0;
}
