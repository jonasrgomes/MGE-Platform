#include <iomanip>
#include <iostream>
#include "mge/ber/ber_encoder.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"

int main()
{
    using namespace mge;
    core::byte memory[128]{};
    core::BufferWriter writer(core::MutableBufferView(memory, sizeof(memory)));
    ber::Encoder encoder(writer);

    const core::byte flags[] = {0xA0};
    if (encoder.writeVisibleString("MGE").failed() ||
        encoder.writeUtf8String("Protecao").failed() ||
        encoder.writeOctetString("ABC", 3).failed() ||
        encoder.writeBitString(flags, 3).failed())
        return 1;

    for (core::usize i = 0; i < writer.position(); ++i)
        std::cout << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<unsigned>(memory[i]) << ' ';
    std::cout << '\n';
    return 0;
}
