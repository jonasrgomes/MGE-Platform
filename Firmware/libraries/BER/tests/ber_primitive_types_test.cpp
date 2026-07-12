#include <cassert>
#include <cstdint>
#include <iostream>

#include "mge/ber/ber_decoder.h"
#include "mge/ber/ber_encoder.h"
#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"

namespace
{

mge::ber::TlvView decodeSingleTlv(
    const mge::core::byte* data,
    mge::core::usize size)
{
    using namespace mge;

    core::BufferReader reader(
        core::BufferView(data, size));

    ber::Decoder decoder(reader);
    ber::TlvView tlv;

    const core::Result result =
        decoder.readTlv(tlv);

    assert(result.ok());
    assert(reader.eof());

    return tlv;
}

} // namespace

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

        assert(encoder.writeBoolean(true).ok());

        assert(writer.position() == 3);
        assert(memory[0] == 0x01);
        assert(memory[1] == 0x01);
        assert(memory[2] == 0xFF);

        const ber::TlvView tlv =
            decodeSingleTlv(
                memory,
                writer.position());

        bool value = false;

        assert(
            ber::Decoder::decodeBoolean(
                tlv,
                value).ok());

        assert(value);
    }

    {
        core::byte memory[32]{};

        core::BufferWriter writer(
            core::MutableBufferView(
                memory,
                sizeof(memory)));

        ber::Encoder encoder(writer);

        assert(encoder.writeInteger(127).ok());
        assert(memory[0] == 0x02);
        assert(memory[1] == 0x01);
        assert(memory[2] == 0x7F);

        const ber::TlvView tlv =
            decodeSingleTlv(
                memory,
                writer.position());

        std::int64_t value = 0;

        assert(
            ber::Decoder::decodeInteger(
                tlv,
                value).ok());

        assert(value == 127);
    }

    {
        core::byte memory[32]{};

        core::BufferWriter writer(
            core::MutableBufferView(
                memory,
                sizeof(memory)));

        ber::Encoder encoder(writer);

        assert(encoder.writeInteger(128).ok());

        assert(memory[0] == 0x02);
        assert(memory[1] == 0x02);
        assert(memory[2] == 0x00);
        assert(memory[3] == 0x80);

        const ber::TlvView tlv =
            decodeSingleTlv(
                memory,
                writer.position());

        std::int64_t value = 0;

        assert(
            ber::Decoder::decodeInteger(
                tlv,
                value).ok());

        assert(value == 128);
    }

    {
        core::byte memory[32]{};

        core::BufferWriter writer(
            core::MutableBufferView(
                memory,
                sizeof(memory)));

        ber::Encoder encoder(writer);

        assert(encoder.writeInteger(-129).ok());

        assert(memory[0] == 0x02);
        assert(memory[1] == 0x02);
        assert(memory[2] == 0xFF);
        assert(memory[3] == 0x7F);

        const ber::TlvView tlv =
            decodeSingleTlv(
                memory,
                writer.position());

        std::int64_t value = 0;

        assert(
            ber::Decoder::decodeInteger(
                tlv,
                value).ok());

        assert(value == -129);
    }

    {
        core::byte memory[16]{};

        core::BufferWriter writer(
            core::MutableBufferView(
                memory,
                sizeof(memory)));

        ber::Encoder encoder(writer);

        assert(encoder.writeEnumerated(3).ok());

        assert(memory[0] == 0x0A);
        assert(memory[1] == 0x01);
        assert(memory[2] == 0x03);

        const ber::TlvView tlv =
            decodeSingleTlv(
                memory,
                writer.position());

        std::int32_t value = 0;

        assert(
            ber::Decoder::decodeEnumerated(
                tlv,
                value).ok());

        assert(value == 3);
    }

    {
        core::byte memory[8]{};

        core::BufferWriter writer(
            core::MutableBufferView(
                memory,
                sizeof(memory)));

        ber::Encoder encoder(writer);

        assert(encoder.writeNull().ok());

        assert(writer.position() == 2);
        assert(memory[0] == 0x05);
        assert(memory[1] == 0x00);

        const ber::TlvView tlv =
            decodeSingleTlv(
                memory,
                writer.position());

        assert(
            ber::Decoder::decodeNull(tlv).ok());
    }

    std::cout
        << "All BER primitive type tests passed.\n";

    return 0;
}
