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

core::Result Encoder::writeBoolean(bool value)
{
    const core::u8 encoded =
        value ? 0xFFU : 0x00U;

    return writeTlv(
        UniversalTag::Boolean,
        &encoded,
        1);
}

core::Result Encoder::writeSignedInteger(
    const Tag& tag,
    std::int64_t value)
{
    core::u8 bytes[8]{};

    const std::uint64_t raw =
        static_cast<std::uint64_t>(value);

    for (core::usize i = 0; i < 8; ++i)
    {
        bytes[7 - i] =
            static_cast<core::u8>(
                (raw >> (i * 8)) & 0xFFU);
    }

    core::usize start = 0;

    if (value >= 0)
    {
        while (start < 7 &&
               bytes[start] == 0x00U &&
               (bytes[start + 1] & 0x80U) == 0)
        {
            ++start;
        }
    }
    else
    {
        while (start < 7 &&
               bytes[start] == 0xFFU &&
               (bytes[start + 1] & 0x80U) != 0)
        {
            ++start;
        }
    }

    return writeTlv(
        tag,
        bytes + start,
        8 - start);
}

core::Result Encoder::writeInteger(std::int64_t value)
{
    return writeSignedInteger(
        UniversalTag::Integer,
        value);
}

core::Result Encoder::writeEnumerated(std::int32_t value)
{
    return writeSignedInteger(
        UniversalTag::Enumerated,
        static_cast<std::int64_t>(value));
}

core::Result Encoder::writeNull()
{
    core::Result result =
        writeTag(UniversalTag::Null);

    if (result.failed())
    {
        return result;
    }

    return writeLength(0);
}


core::Result Encoder::writeOctetString(const void* data, core::usize length)
{
    if (data == nullptr && length != 0)
        return core::Error::InvalidPointer;
    return writeTlv(UniversalTag::OctetString, data, length);
}

core::Result Encoder::writeVisibleString(const char* text, core::usize length)
{
    if (text == nullptr && length != 0)
        return core::Error::InvalidPointer;
    return writeTlv(UniversalTag::VisibleString, text, length);
}

core::Result Encoder::writeVisibleString(const char* nullTerminatedText)
{
    if (nullTerminatedText == nullptr)
        return core::Error::InvalidPointer;

    core::usize length = 0;
    while (nullTerminatedText[length] != '\0')
        ++length;

    return writeVisibleString(nullTerminatedText, length);
}

core::Result Encoder::writeUtf8String(const char* text, core::usize length)
{
    if (text == nullptr && length != 0)
        return core::Error::InvalidPointer;
    return writeTlv(UniversalTag::Utf8String, text, length);
}

core::Result Encoder::writeUtf8String(const char* nullTerminatedText)
{
    if (nullTerminatedText == nullptr)
        return core::Error::InvalidPointer;

    core::usize length = 0;
    while (nullTerminatedText[length] != '\0')
        ++length;

    return writeUtf8String(nullTerminatedText, length);
}

core::Result Encoder::writeBitString(const void* data, core::usize bitCount)
{
    const core::usize byteCount = (bitCount + 7U) / 8U;

    if (data == nullptr && byteCount != 0)
        return core::Error::InvalidPointer;

    const core::u8 unusedBits =
        static_cast<core::u8>(byteCount == 0 ? 0 : (byteCount * 8U - bitCount));

    core::Result result = writeTag(UniversalTag::BitString);
    if (result.failed()) return result;

    result = writeLength(byteCount + 1U);
    if (result.failed()) return result;

    result = m_writer.writeU8(unusedBits);
    if (result.failed()) return result;

    return m_writer.writeBytes(data, byteCount);
}

} // namespace ber
} // namespace mge
