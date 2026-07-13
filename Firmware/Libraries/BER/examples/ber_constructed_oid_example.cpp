#include <iomanip>
#include <iostream>
#include "mge/ber/ber_encoder.h"
#include "mge/core/buffer_writer.h"

int main()
{
    using namespace mge;
    core::byte output[128]{};
    core::BufferWriter writer(core::MutableBufferView(output, sizeof(output)));
    ber::Encoder encoder(writer);
    const std::uint32_t oid[] = {1,3,6,1,4,1,55555,1};
    if (encoder.writeObjectIdentifier(oid, 8).failed()) return 1;
    for (core::usize i=0; i<writer.position(); ++i)
        std::cout << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<unsigned>(output[i]) << ' ';
    std::cout << '\n';
    return 0;
}
