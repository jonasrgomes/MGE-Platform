#include "mge/goose/goose_apdu_encoder.h"

namespace mge { namespace goose {

core::usize ApduEncoder::textLength(const char* text) noexcept
{
    if (text == nullptr) return 0;
    core::usize length = 0;
    while (text[length] != '\0') ++length;
    return length;
}

core::Result ApduEncoder::writeContextBytes(
    core::BufferWriter& writer,
    core::u32 tagNumber,
    ber::PcBit pc,
    const void* data,
    core::usize size)
{
    ber::Encoder encoder(writer);
    return encoder.writeContextSpecific(
        tagNumber,
        pc,
        core::BufferView(
            static_cast<const core::byte*>(data),
            size));
}

core::Result ApduEncoder::writeContextBoolean(
    core::BufferWriter& writer,
    core::u32 tagNumber,
    bool value)
{
    const core::byte encoded = value ? 0xFFU : 0x00U;
    return writeContextBytes(
        writer, tagNumber, ber::PcBit::Primitive, &encoded, 1);
}

core::Result ApduEncoder::writeContextUnsigned(
    core::BufferWriter& writer,
    core::u32 tagNumber,
    core::u64 value)
{
    core::byte bytes[9]{};
    core::usize start = 9;

    do
    {
        bytes[--start] = static_cast<core::byte>(value & 0xFFU);
        value >>= 8;
    }
    while (value != 0);

    if ((bytes[start] & 0x80U) != 0)
        bytes[--start] = 0x00U;

    return writeContextBytes(
        writer, tagNumber, ber::PcBit::Primitive,
        bytes + start, 9 - start);
}

core::Result ApduEncoder::writeContextSigned(
    core::BufferWriter& writer,
    core::u32 tagNumber,
    core::s64 value)
{
    core::byte bytes[8]{};
    const core::u64 raw = static_cast<core::u64>(value);

    for (core::usize i = 0; i < 8; ++i)
        bytes[7 - i] = static_cast<core::byte>((raw >> (i * 8)) & 0xFFU);

    core::usize start = 0;
    if (value >= 0)
    {
        while (start < 7 && bytes[start] == 0x00U &&
               (bytes[start + 1] & 0x80U) == 0)
            ++start;
    }
    else
    {
        while (start < 7 && bytes[start] == 0xFFU &&
               (bytes[start + 1] & 0x80U) != 0)
            ++start;
    }

    return writeContextBytes(
        writer, tagNumber, ber::PcBit::Primitive,
        bytes + start, 8 - start);
}

core::Result ApduEncoder::encodeDataValue(
    const asn1::Value& value,
    core::BufferWriter& writer)
{
    switch (value.type())
    {
        case asn1::Type::Boolean:
            return writeContextBoolean(writer, 3, value.booleanValue());

        case asn1::Type::Integer:
            return writeContextSigned(writer, 5, value.integerValue());

        case asn1::Type::BitString:
        {
            const core::BufferView bytes = value.bytes();
            const core::usize unusedBits =
                bytes.size() * 8U - value.bitCount();

            if (unusedBits > 7U)
                return core::Error::InvalidValue;

            core::byte encoded[256]{};
            if (bytes.size() + 1U > sizeof(encoded))
                return core::Error::BufferOverflow;

            encoded[0] = static_cast<core::byte>(unusedBits);
            for (core::usize i = 0; i < bytes.size(); ++i)
                encoded[i + 1] = bytes[i];

            return writeContextBytes(
                writer, 4, ber::PcBit::Primitive,
                encoded, bytes.size() + 1U);
        }

        case asn1::Type::OctetString:
            return writeContextBytes(
                writer, 9, ber::PcBit::Primitive,
                value.bytes().data(), value.bytes().size());

        case asn1::Type::VisibleString:
            return writeContextBytes(
                writer, 10, ber::PcBit::Primitive,
                value.bytes().data(), value.bytes().size());

        case asn1::Type::Utf8String:
            return writeContextBytes(
                writer, 16, ber::PcBit::Primitive,
                value.bytes().data(), value.bytes().size());

        case asn1::Type::Null:
        default:
            return core::Error::Unsupported;
    }
}

}} // namespace mge::goose
