/******************************************************************************
 *
 * MGE Platform
 * Industrial Automation Framework
 *
 * @file        buffer_reader.cpp
 * @brief       Implementation of the sequential binary buffer reader.
 *
 ******************************************************************************/

#include "mge/core/buffer_reader.h"

#include <cstring>

namespace mge
{
namespace core
{

BufferReader::BufferReader(const BufferView& buffer)
    : m_buffer(buffer),
      m_position(0)
{
}

//=============================================================================
// Position
//=============================================================================

usize BufferReader::position() const noexcept
{
    return m_position;
}

usize BufferReader::remaining() const noexcept
{
    if (m_position >= m_buffer.size())
    {
        return 0;
    }

    return m_buffer.size() - m_position;
}

usize BufferReader::size() const noexcept
{
    return m_buffer.size();
}

const byte* BufferReader::currentData() const noexcept
{
    if (m_buffer.data() == nullptr || m_position > m_buffer.size())
    {
        return nullptr;
    }

    return m_buffer.data() + m_position;
}

bool BufferReader::eof() const noexcept
{
    return m_position >= m_buffer.size();
}

void BufferReader::rewind() noexcept
{
    m_position = 0;
}

Result BufferReader::seek(usize position)
{
    if (position > m_buffer.size())
    {
        return Error::InvalidParameter;
    }

    m_position = position;
    return Error::Ok;
}

Result BufferReader::skip(usize bytes)
{
    if (bytes > remaining())
    {
        return Error::BufferUnderflow;
    }

    m_position += bytes;
    return Error::Ok;
}

//=============================================================================
// Peek
//=============================================================================

Result BufferReader::peekU8(uint8_t& value) const
{
    if (remaining() < sizeof(uint8_t))
    {
        return Error::BufferUnderflow;
    }

    value = m_buffer.data()[m_position];
    return Error::Ok;
}

Result BufferReader::peekU16(uint16_t& value) const
{
    if (remaining() < sizeof(uint16_t))
    {
        return Error::BufferUnderflow;
    }

    const byte* data = m_buffer.data() + m_position;

    value =
        (static_cast<uint16_t>(data[0]) << 8) |
        static_cast<uint16_t>(data[1]);

    return Error::Ok;
}

Result BufferReader::peekU32(uint32_t& value) const
{
    if (remaining() < sizeof(uint32_t))
    {
        return Error::BufferUnderflow;
    }

    const byte* data = m_buffer.data() + m_position;

    value =
        (static_cast<uint32_t>(data[0]) << 24) |
        (static_cast<uint32_t>(data[1]) << 16) |
        (static_cast<uint32_t>(data[2]) << 8) |
        static_cast<uint32_t>(data[3]);

    return Error::Ok;
}

//=============================================================================
// Read
//=============================================================================

Result BufferReader::readU8(uint8_t& value)
{
    Result result = peekU8(value);

    if (result.failed())
    {
        return result;
    }

    m_position += sizeof(uint8_t);
    return Error::Ok;
}

Result BufferReader::readU16(uint16_t& value)
{
    Result result = peekU16(value);

    if (result.failed())
    {
        return result;
    }

    m_position += sizeof(uint16_t);
    return Error::Ok;
}

Result BufferReader::readU32(uint32_t& value)
{
    Result result = peekU32(value);

    if (result.failed())
    {
        return result;
    }

    m_position += sizeof(uint32_t);
    return Error::Ok;
}

Result BufferReader::readU64(uint64_t& value)
{
    if (remaining() < sizeof(uint64_t))
    {
        return Error::BufferUnderflow;
    }

    const byte* data = m_buffer.data() + m_position;

    value =
        (static_cast<uint64_t>(data[0]) << 56) |
        (static_cast<uint64_t>(data[1]) << 48) |
        (static_cast<uint64_t>(data[2]) << 40) |
        (static_cast<uint64_t>(data[3]) << 32) |
        (static_cast<uint64_t>(data[4]) << 24) |
        (static_cast<uint64_t>(data[5]) << 16) |
        (static_cast<uint64_t>(data[6]) << 8) |
        static_cast<uint64_t>(data[7]);

    m_position += sizeof(uint64_t);
    return Error::Ok;
}

Result BufferReader::readBool(bool& value)
{
    uint8_t encodedValue = 0;

    Result result = readU8(encodedValue);

    if (result.failed())
    {
        return result;
    }

    value = encodedValue != 0;
    return Error::Ok;
}

Result BufferReader::readFloat(float& value)
{
    static_assert(
        sizeof(float) == sizeof(uint32_t),
        "float must have 32 bits"
    );

    uint32_t encodedValue = 0;

    Result result = readU32(encodedValue);

    if (result.failed())
    {
        return result;
    }

    std::memcpy(&value, &encodedValue, sizeof(value));
    return Error::Ok;
}

Result BufferReader::readDouble(double& value)
{
    static_assert(
        sizeof(double) == sizeof(uint64_t),
        "double must have 64 bits"
    );

    uint64_t encodedValue = 0;

    Result result = readU64(encodedValue);

    if (result.failed())
    {
        return result;
    }

    std::memcpy(&value, &encodedValue, sizeof(value));
    return Error::Ok;
}

Result BufferReader::readBytes(void* destination, usize size)
{
    if (destination == nullptr && size != 0)
    {
        return Error::InvalidPointer;
    }

    if (size > remaining())
    {
        return Error::BufferUnderflow;
    }

    if (size != 0)
    {
        std::memcpy(
            destination,
            m_buffer.data() + m_position,
            size
        );
    }

    m_position += size;
    return Error::Ok;
}

Result BufferReader::readString(
    char* destination,
    usize maxSize
)
{
    if (destination == nullptr)
    {
        return Error::InvalidPointer;
    }

    if (maxSize == 0)
    {
        return Error::InvalidParameter;
    }

    usize destinationPosition = 0;

    while (!eof())
    {
        uint8_t character = 0;

        Result result = readU8(character);

        if (result.failed())
        {
            return result;
        }

        if (character == '\0')
        {
            destination[destinationPosition] = '\0';
            return Error::Ok;
        }

        if (destinationPosition + 1 >= maxSize)
        {
            destination[maxSize - 1] = '\0';
            return Error::BufferOverflow;
        }

        destination[destinationPosition++] =
            static_cast<char>(character);
    }

    destination[destinationPosition] = '\0';
    return Error::BufferUnderflow;
}

} // namespace core
} // namespace mge