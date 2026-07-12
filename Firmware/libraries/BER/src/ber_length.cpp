#include "mge/ber/ber_length.h"

namespace mge
{
namespace ber
{

core::Result Length::encode(
    core::usize value,
    core::BufferWriter& writer)
{
    if (value < 128)
    {
        return writer.writeU8(
            static_cast<core::u8>(value));
    }

    core::u8 bytes[sizeof(core::usize)]{};
    core::usize count = 0;
    core::usize temp = value;

    while (temp != 0)
    {
        bytes[count++] =
            static_cast<core::u8>(temp & 0xFFU);
        temp >>= 8;
    }

    core::Result result =
        writer.writeU8(
            static_cast<core::u8>(0x80U | count));

    if (result.failed())
    {
        return result;
    }

    while (count != 0)
    {
        result = writer.writeU8(bytes[--count]);

        if (result.failed())
        {
            return result;
        }
    }

    return core::Error::Ok;
}

core::Result Length::decode(
    core::BufferReader& reader,
    core::usize& value)
{
    core::u8 first = 0;

    core::Result result = reader.readU8(first);

    if (result.failed())
    {
        return result;
    }

    if ((first & 0x80U) == 0)
    {
        value = first;
        return core::Error::Ok;
    }

    const core::u8 count =
        static_cast<core::u8>(first & 0x7FU);

    if (count == 0)
    {
        return core::Error::Unsupported;
    }

    if (count > sizeof(core::usize))
    {
        return core::Error::InvalidLength;
    }

    value = 0;

    for (core::u8 i = 0; i < count; ++i)
    {
        core::u8 current = 0;

        result = reader.readU8(current);

        if (result.failed())
        {
            return result;
        }

        value = (value << 8) | current;
    }

    return core::Error::Ok;
}

} // namespace ber
} // namespace mge
