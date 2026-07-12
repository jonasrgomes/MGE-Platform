#include <cassert>
#include <iostream>

#include "mge/core/buffer_view.h"
#include "mge/ethernet/ethernet_parser.h"

int main()
{
    using namespace mge;

    core::byte rawFrame[] = {
        0x01,0x0C,0xCD,0x01,0x00,0x02,
        0x02,0x12,0x34,0x56,0x78,0x01,
        0x88,0xB8,
        0x10,0x02,0x00,0x08,0x00,0x00,0x00,0x00
    };

    ethernet::EthernetFrame frame;
    auto result = ethernet::EthernetParser::parse(
        core::BufferView(rawFrame, sizeof(rawFrame)), frame);

    assert(result.ok());
    assert(frame.etherType() == ethernet::EtherType::GOOSE);
    assert(frame.payload().size() == 8);
    assert(frame.payload().data() == rawFrame + 14);

    core::byte shortFrame[13]{};
    ethernet::EthernetFrame invalidFrame;
    auto invalid = ethernet::EthernetParser::parse(
        core::BufferView(shortFrame, sizeof(shortFrame)), invalidFrame);

    assert(invalid.failed());
    assert(invalid.error == core::Error::InvalidLength);

    std::cout << "All EthernetParser tests passed.\n";
    return 0;
}
