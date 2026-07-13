#include <cassert>
#include <cstring>
#include <iostream>
#include "mge/ber/ber_decoder.h"
#include "mge/ber/ber_encoder.h"
#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"

namespace {
mge::ber::TlvView decode(const mge::core::byte* data, mge::core::usize size)
{
    mge::core::BufferReader reader(mge::core::BufferView(data, size));
    mge::ber::Decoder decoder(reader);
    mge::ber::TlvView tlv;
    assert(decoder.readTlv(tlv).ok());
    assert(reader.eof());
    return tlv;
}
}

int main()
{
    using namespace mge;

    {
        core::byte mem[32]{};
        core::BufferWriter writer(core::MutableBufferView(mem, sizeof(mem)));
        ber::Encoder encoder(writer);
        const core::byte bytes[] = {0x10, 0x20, 0x30};
        assert(encoder.writeOctetString(bytes, sizeof(bytes)).ok());
        assert(mem[0] == 0x04 && mem[1] == 0x03);
        auto tlv = decode(mem, writer.position());
        core::BufferView value;
        assert(ber::Decoder::decodeOctetString(tlv, value).ok());
        assert(value.size() == 3 && value[2] == 0x30);
    }

    {
        core::byte mem[32]{};
        core::BufferWriter writer(core::MutableBufferView(mem, sizeof(mem)));
        ber::Encoder encoder(writer);
        assert(encoder.writeVisibleString("MGE").ok());
        assert(mem[0] == 0x1A && mem[1] == 0x03);
        auto tlv = decode(mem, writer.position());
        core::BufferView value;
        assert(ber::Decoder::decodeVisibleString(tlv, value).ok());
        assert(value.size() == 3);
        assert(value[0] == 'M' && value[1] == 'G' && value[2] == 'E');
    }

    {
        core::byte mem[64]{};
        core::BufferWriter writer(core::MutableBufferView(mem, sizeof(mem)));
        ber::Encoder encoder(writer);
        const char utf8[] = "MGE \xC3\xA7";
        assert(encoder.writeUtf8String(utf8, sizeof(utf8) - 1).ok());
        assert(mem[0] == 0x0C);
        auto tlv = decode(mem, writer.position());
        core::BufferView value;
        assert(ber::Decoder::decodeUtf8String(tlv, value).ok());
        assert(value.size() == sizeof(utf8) - 1);
    }

    {
        core::byte mem[32]{};
        core::BufferWriter writer(core::MutableBufferView(mem, sizeof(mem)));
        ber::Encoder encoder(writer);
        const core::byte bits[] = {0b10110000};
        assert(encoder.writeBitString(bits, 4).ok());
        assert(mem[0] == 0x03 && mem[1] == 0x02 && mem[2] == 0x04 && mem[3] == 0xB0);
        auto tlv = decode(mem, writer.position());
        ber::BitStringView value;
        assert(ber::Decoder::decodeBitString(tlv, value).ok());
        assert(value.bitCount == 4 && value.unusedBits == 4);
        bool b = false;
        assert(value.bit(0, b) && b);
        assert(value.bit(1, b) && !b);
        assert(value.bit(2, b) && b);
        assert(value.bit(3, b) && b);
        assert(!value.bit(4, b));
    }

    std::cout << "All BER string type tests passed.\n";
    return 0;
}
