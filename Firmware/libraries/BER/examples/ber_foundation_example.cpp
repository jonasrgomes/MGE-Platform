#include <iomanip>
#include <iostream>

#include "mge/ber/ber_encoder.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"

int main()
{
    using namespace mge;

    core::byte memory[64]{};

    core::BufferWriter writer(
        core::MutableBufferView(
            memory,
            sizeof(memory)));

    ber::Encoder encoder(writer);

    const char text[] = "MGE";

    const core::Result result =
        encoder.writeTlv(
            ber::UniversalTag::VisibleString,
            text,
            3);

    if (result.failed())
    {
        return 1;
    }

    for (core::usize i = 0;
         i < writer.position();
         ++i)
    {
        std::cout
            << std::hex
            << std::uppercase
            << std::setw(2)
            << std::setfill('0')
            << static_cast<unsigned>(memory[i])
            << ' ';
    }

    std::cout << '\n';
    return 0;
}
