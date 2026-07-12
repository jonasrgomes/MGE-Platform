#include <iostream>
#include "mge/core/buffer_view.h"
#include "mge/ethernet/ethernet_parser.h"

int main()
{
    using namespace mge;

    core::byte received[] = {
        0x01,0x0C,0xCD,0x01,0x00,0x01,
        0x02,0x12,0x34,0x56,0x78,0x01,
        0x88,0xB8,
        0x10,0x01,0x00,0x08,0x00,0x00,0x00,0x00
    };

    ethernet::EthernetFrame frame;
    auto result = ethernet::EthernetParser::parse(
        core::BufferView(received, sizeof(received)), frame);

    if (result.failed())
        return 1;

    char destination[18]{};
    char source[18]{};

    frame.destination().toString(destination, sizeof(destination));
    frame.source().toString(source, sizeof(source));

    std::cout << "Destino: " << destination << '\n';
    std::cout << "Origem : " << source << '\n';
    std::cout << "Payload: " << frame.payload().size() << " bytes\n";
    return 0;
}
