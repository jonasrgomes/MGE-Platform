#include <cassert>
#include <iostream>

#include "mge/ber/ber_decoder.h"
#include "mge/ber/ber_encoder.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"

int main()
{
    using namespace mge;

    {
        core::byte memory[16]{};
        core::BufferWriter writer(
            core::MutableBufferView(
                memory,
                sizeof(memory)));

        ber::Encoder encoder(writer);

        const core::byte value = 0xFF;

        auto result =
            encoder.writeTlv(
                ber::UniversalTag::Boolean,
                &value,
                1);

        assert(result.ok());
        assert(writer.position() == 3);
        assert(memory[0] == 0x01);
        assert(memory[1] == 0x01);
        assert(memory[2] == 0xFF);
    }

    {
        core::byte memory[256]{};
        core::BufferWriter writer(
            core::MutableBufferView(
                memory,
                sizeof(memory)));

        auto result =
            ber::Length::encode(130, writer);

        assert(result.ok());
        assert(writer.position() == 2);
        assert(memory[0] == 0x81);
        assert(memory[1] == 0x82);

        core::BufferReader reader(
            core::BufferView(
                memory,
                writer.position()));

        core::usize decoded = 0;

        result = ber::Length::decode(
            reader,
            decoded);

        assert(result.ok());
        assert(decoded == 130);
    }

    {
        core::byte encoded[] =
        {
            0x1A, 0x03, 'M', 'G', 'E'
        };

        core::BufferReader reader(
            core::BufferView(
                encoded,
                sizeof(encoded)));

        ber::Decoder decoder(reader);
        ber::TlvView tlv;

        const auto result =
            decoder.readTlv(tlv);

        assert(result.ok());
        assert(tlv.tag.number == 26);
        assert(tlv.value.size() == 3);
        assert(tlv.value[0] == 'M');
        assert(tlv.value[1] == 'G');
        assert(tlv.value[2] == 'E');
    }

    std::cout
        << "All BER Foundation tests passed.\n";

    return 0;
}
