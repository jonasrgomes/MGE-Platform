#include <cassert>
#include <cstring>
#include <iostream>

#include "mge/asn1/asn1_decoder.h"
#include "mge/asn1/asn1_encoder.h"
#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_writer.h"

namespace
{

mge::asn1::Value roundTrip(const mge::asn1::Value& input,
                           mge::core::byte* memory,
                           mge::core::usize capacity)
{
    using namespace mge;

    core::BufferWriter writer(core::MutableBufferView(memory, capacity));
    ber::Encoder berEncoder(writer);
    asn1::Encoder encoder(berEncoder);

    assert(encoder.encode(input).ok());

    core::BufferReader reader(core::BufferView(memory, writer.position()));
    ber::Decoder berDecoder(reader);
    ber::TlvView tlv;

    assert(berDecoder.readTlv(tlv).ok());
    assert(reader.eof());

    asn1::Value output;
    assert(asn1::Decoder::decode(tlv, output).ok());
    return output;
}

} // namespace

int main()
{
    using namespace mge;

    core::byte memory[128]{};

    {
        const asn1::Value output =
            roundTrip(asn1::Value::boolean(true), memory, sizeof(memory));
        assert(output.is(asn1::Type::Boolean));
        assert(output.booleanValue());
    }

    {
        const asn1::Value output =
            roundTrip(asn1::Value::integer(-12345), memory, sizeof(memory));
        assert(output.is(asn1::Type::Integer));
        assert(output.integerValue() == -12345);
    }

    {
        const core::byte source[] = {0x10, 0x20, 0x30};
        const asn1::Value output = roundTrip(
            asn1::Value::octetString(core::BufferView(source, sizeof(source))),
            memory, sizeof(memory));
        assert(output.is(asn1::Type::OctetString));
        assert(output.bytes().size() == 3);
        assert(output.bytes()[1] == 0x20);
    }

    {
        const char text[] = "MGE";
        const asn1::Value output = roundTrip(
            asn1::Value::visibleString(core::BufferView(
                reinterpret_cast<const core::byte*>(text), 3)),
            memory, sizeof(memory));
        assert(output.is(asn1::Type::VisibleString));
        assert(output.bytes().size() == 3);
        assert(std::memcmp(output.bytes().data(), "MGE", 3) == 0);
    }

    {
        const core::byte bits[] = {0b10100000};
        const asn1::Value output = roundTrip(
            asn1::Value::bitString(core::BufferView(bits, 1), 3),
            memory, sizeof(memory));
        assert(output.is(asn1::Type::BitString));
        assert(output.bitCount() == 3);
        assert(output.bytes()[0] == 0b10100000);
    }

    {
        const asn1::Value output =
            roundTrip(asn1::Value::null(), memory, sizeof(memory));
        assert(output.is(asn1::Type::Null));
    }

    std::cout << "All ASN.1 Foundation tests passed.\n";
    return 0;
}
