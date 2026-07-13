#include "mge/ber/ber_decoder.h"

#include <limits>

namespace mge
{
namespace ber
{

core::Result Decoder::readTag(Tag& tag)
{
    core::u8 first = 0;

    core::Result result = m_reader.readU8(first);

    if (result.failed())
    {
        return result;
    }

    tag.tagClass =
        static_cast<TagClass>(first & 0xC0U);

    tag.pc =
        static_cast<PcBit>(first & 0x20U);

    core::u32 number = first & 0x1FU;

    if (number != 0x1FU)
    {
        tag.number = number;
        return core::Error::Ok;
    }

    number = 0;
    core::u8 current = 0;
    core::u8 groups = 0;

    do
    {
        result = m_reader.readU8(current);

        if (result.failed())
        {
            return result;
        }

        if (++groups > 5)
        {
            return core::Error::InvalidTag;
        }

        number =
            (number << 7) |
            static_cast<core::u32>(
                current & 0x7FU);
    }
    while ((current & 0x80U) != 0);

    tag.number = number;
    return core::Error::Ok;
}

core::Result Decoder::readLength(core::usize& length)
{
    return Length::decode(m_reader, length);
}

core::Result Decoder::readTlv(TlvView& tlv)
{
    core::Result result = readTag(tlv.tag);

    if (result.failed())
    {
        return result;
    }

    core::usize length = 0;

    result = readLength(length);

    if (result.failed())
    {
        return result;
    }

    if (length > m_reader.remaining())
    {
        return core::Error::BufferUnderflow;
    }

    tlv.value =
        core::BufferView(
            m_reader.currentData(),
            length);

    return m_reader.skip(length);
}

core::Result Decoder::decodeBoolean(
    const TlvView& tlv,
    bool& value)
{
    if (tlv.tag != UniversalTag::Boolean)
    {
        return core::Error::InvalidTag;
    }

    if (tlv.value.size() != 1)
    {
        return core::Error::InvalidLength;
    }

    value = tlv.value[0] != 0;
    return core::Error::Ok;
}

core::Result Decoder::decodeSignedIntegerValue(
    const core::BufferView& bytes,
    std::int64_t& value)
{
    if (bytes.data() == nullptr)
    {
        return core::Error::InvalidPointer;
    }

    if (bytes.size() == 0 || bytes.size() > 8)
    {
        return core::Error::InvalidLength;
    }

    std::uint64_t raw = 0;

    for (core::usize i = 0; i < bytes.size(); ++i)
    {
        raw =
            (raw << 8) |
            static_cast<std::uint64_t>(bytes[i]);
    }

    if ((bytes[0] & 0x80U) != 0 &&
        bytes.size() < 8)
    {
        const core::usize usedBits =
            bytes.size() * 8;

        raw |=
            (~std::uint64_t{0}) << usedBits;
    }

    value = static_cast<std::int64_t>(raw);
    return core::Error::Ok;
}

core::Result Decoder::decodeInteger(
    const TlvView& tlv,
    std::int64_t& value)
{
    if (tlv.tag != UniversalTag::Integer)
    {
        return core::Error::InvalidTag;
    }

    return decodeSignedIntegerValue(
        tlv.value,
        value);
}

core::Result Decoder::decodeEnumerated(
    const TlvView& tlv,
    std::int32_t& value)
{
    if (tlv.tag != UniversalTag::Enumerated)
    {
        return core::Error::InvalidTag;
    }

    std::int64_t decoded = 0;

    core::Result result =
        decodeSignedIntegerValue(
            tlv.value,
            decoded);

    if (result.failed())
    {
        return result;
    }

    if (decoded < std::numeric_limits<std::int32_t>::min() ||
        decoded > std::numeric_limits<std::int32_t>::max())
    {
        return core::Error::InvalidValue;
    }

    value = static_cast<std::int32_t>(decoded);
    return core::Error::Ok;
}

core::Result Decoder::decodeNull(
    const TlvView& tlv)
{
    if (tlv.tag != UniversalTag::Null)
    {
        return core::Error::InvalidTag;
    }

    if (!tlv.value.empty())
    {
        return core::Error::InvalidLength;
    }

    return core::Error::Ok;
}


core::Result Decoder::decodeOctetString(const TlvView& tlv,
                                        core::BufferView& value)
{
    if (tlv.tag != UniversalTag::OctetString)
        return core::Error::InvalidTag;
    value = tlv.value;
    return core::Error::Ok;
}

core::Result Decoder::decodeVisibleString(const TlvView& tlv,
                                          core::BufferView& value)
{
    if (tlv.tag != UniversalTag::VisibleString)
        return core::Error::InvalidTag;
    value = tlv.value;
    return core::Error::Ok;
}

core::Result Decoder::decodeUtf8String(const TlvView& tlv,
                                       core::BufferView& value)
{
    if (tlv.tag != UniversalTag::Utf8String)
        return core::Error::InvalidTag;
    value = tlv.value;
    return core::Error::Ok;
}

core::Result Decoder::decodeBitString(const TlvView& tlv,
                                      BitStringView& value)
{
    if (tlv.tag != UniversalTag::BitString)
        return core::Error::InvalidTag;

    if (tlv.value.size() == 0)
        return core::Error::InvalidLength;

    const core::u8 unusedBits = tlv.value[0];
    if (unusedBits > 7)
        return core::Error::InvalidValue;

    const core::usize dataBytes = tlv.value.size() - 1U;
    if (dataBytes == 0 && unusedBits != 0)
        return core::Error::InvalidValue;

    const core::BufferView bytes = tlv.value.subview(1, dataBytes);

    if (dataBytes != 0 && unusedBits != 0)
    {
        const core::u8 mask = static_cast<core::u8>((1U << unusedBits) - 1U);
        if ((bytes[dataBytes - 1U] & mask) != 0)
            return core::Error::InvalidValue;
    }

    value.bytes = bytes;
    value.unusedBits = unusedBits;
    value.bitCount = dataBytes * 8U - unusedBits;
    return core::Error::Ok;
}

} // namespace ber
} // namespace mge
