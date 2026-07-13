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

namespace mge { namespace ber {

core::Result Decoder::decodeConstructed(
    const TlvView& tlv,
    const Tag& expectedTag,
    core::BufferView& content)
{
    if (tlv.tag != expectedTag) return core::Error::InvalidTag;
    if (tlv.tag.pc != PcBit::Constructed) return core::Error::InvalidType;
    content = tlv.value;
    return core::Error::Ok;
}

core::Result Decoder::decodeObjectIdentifier(
    const TlvView& tlv,
    std::uint32_t* arcs,
    core::usize capacity,
    core::usize& arcCount)
{
    arcCount = 0;
    if (tlv.tag != UniversalTag::ObjectIdentifier)
        return core::Error::InvalidTag;
    if (arcs == nullptr) return core::Error::InvalidPointer;
    if (capacity < 2) return core::Error::BufferOverflow;
    if (tlv.value.empty()) return core::Error::InvalidLength;

    std::uint32_t subids[32]{};
    core::usize subCount = 0;
    std::uint32_t current = 0;
    bool continuation = false;

    for (core::usize i = 0; i < tlv.value.size(); ++i)
    {
        const core::u8 byte = tlv.value[i];
        if (current > (0xFFFFFFFFU >> 7)) return core::Error::InvalidValue;
        current = (current << 7) | static_cast<std::uint32_t>(byte & 0x7FU);
        continuation = (byte & 0x80U) != 0;
        if (!continuation)
        {
            if (subCount >= 32) return core::Error::BufferOverflow;
            subids[subCount++] = current;
            current = 0;
        }
    }

    if (continuation || subCount == 0) return core::Error::DecodeError;

    const std::uint32_t first = subids[0];
    if (first < 40)
    {
        arcs[0] = 0; arcs[1] = first;
    }
    else if (first < 80)
    {
        arcs[0] = 1; arcs[1] = first - 40;
    }
    else
    {
        arcs[0] = 2; arcs[1] = first - 80;
    }
    arcCount = 2;

    for (core::usize i = 1; i < subCount; ++i)
    {
        if (arcCount >= capacity) return core::Error::BufferOverflow;
        arcs[arcCount++] = subids[i];
    }

    return core::Error::Ok;
}

}} // namespace mge::ber
