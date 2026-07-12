#include "mge/ber/ber_decoder.h"

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

    const core::byte* valueStart =
        m_reader.currentData();

    tlv.value =
        core::BufferView(valueStart, length);

    return m_reader.skip(length);
}

} // namespace ber
} // namespace mge
