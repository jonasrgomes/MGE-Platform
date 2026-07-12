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

    if (encoder.writeBoolean(true).failed() ||
        encoder.writeInteger(1500).failed() ||
        encoder.writeEnumerated(2).failed() ||
        encoder.writeNull().failed())
    {
        std::cerr << "Falha na codificação BER.\n";
        return 1;
    }

    std::cout << "BER encoded bytes:\n";

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
