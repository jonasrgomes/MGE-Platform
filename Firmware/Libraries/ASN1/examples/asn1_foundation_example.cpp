#include <iomanip>
#include <iostream>

#include "mge/asn1/asn1_encoder.h"
#include "mge/core/buffer_writer.h"

int main()
{
    using namespace mge;

    core::byte memory[64]{};
    core::BufferWriter writer(core::MutableBufferView(memory, sizeof(memory)));
    ber::Encoder berEncoder(writer);
    asn1::Encoder encoder(berEncoder);

    const char text[] = "MGE";

    if (encoder.encode(asn1::Value::boolean(true)).failed() ||
        encoder.encode(asn1::Value::integer(1500)).failed() ||
        encoder.encode(asn1::Value::visibleString(core::BufferView(
            reinterpret_cast<const core::byte*>(text), 3))).failed())
    {
        return 1;
    }

    for (core::usize i = 0; i < writer.position(); ++i)
    {
        std::cout << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<unsigned>(memory[i]) << ' ';
    }

    std::cout << '\n';
    return 0;
}
