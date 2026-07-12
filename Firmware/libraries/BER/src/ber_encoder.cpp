#include "mge/ber/ber_encoder.h"

namespace mge
{
namespace ber
{

core::Result Encoder::writeTag(const Tag& tag)
{
    core::Result result =
        m_writer.writeU8(tag.firstOctet());

    if (result.failed())
    {
        return result;
    }

    if (tag.isShortForm())
    {
        return core::Error::Ok;
    }

    core::u8 encoded[5]{};
    core::usize count = 0;
    core::u32 number = tag.number;

    do
    {
        encoded[count++] =
            static_cast<core::u8>(number & 0x7FU);
        number >>= 7;
    }
    while (number != 0);

    while (count != 0)
    {
        const bool hasMore = count > 1;

        core::u8 current = encoded[--count];

        if (hasMore)
        {
            current =
                static_cast<core::u8>(current | 0x80U);
        }

        result = m_writer.writeU8(current);

        if (result.failed())
        {
            return result;
        }
    }

    return core::Error::Ok;
}

core::Result Encoder::writeLength(core::usize length)
{
    return Length::encode(length, m_writer);
}

core::Result Encoder::writeTlv(
    const Tag& tag,
    const void* value,
    core::usize length)
{
    core::Result result = writeTag(tag);

    if (result.failed())
    {
        return result;
    }

    result = writeLength(length);

    if (result.failed())
    {
        return result;
    }

    return m_writer.writeBytes(value, length);
}

} // namespace ber
} // namespace mge
