/******************************************************************************
 *
 * MGE Platform
 * Industrial Automation Framework
 *
 * @file        buffer_writer.cpp
 * @brief       Implementação do escritor sequencial de dados binários.
 *
 ******************************************************************************/

#include "mge/core/buffer_writer.h"

#include <cstring>

namespace mge
{
namespace core
{

BufferWriter::BufferWriter(MutableBufferView buffer)
    : m_buffer(buffer),
      m_position(0),
      m_endian(Endian::Big)
{
}

//=============================================================================
// Position
//=============================================================================

usize BufferWriter::position() const noexcept
{
    return m_position;
}

usize BufferWriter::remaining() const noexcept
{
    if (m_position >= m_buffer.size())
    {
        return 0;
    }

    return m_buffer.size() - m_position;
}

usize BufferWriter::size() const noexcept
{
    return m_buffer.size();
}

bool BufferWriter::full() const noexcept
{
    return m_position >= m_buffer.size();
}

void BufferWriter::rewind() noexcept
{
    m_position = 0;
}

Result BufferWriter::seek(usize position)
{
    if (position > m_buffer.size())
    {
        return Error::InvalidParameter;
    }

    m_position = position;

    return Error::Ok;
}

Result BufferWriter::skip(usize bytes)
{
    if (bytes > remaining())
    {
        return Error::BufferOverflow;
    }

    m_position += bytes;

    return Error::Ok;
}

//=============================================================================
// Write
//=============================================================================

Result BufferWriter::writeU8(uint8_t value)
{
    if (remaining() < sizeof(uint8_t))
    {
        return Error::BufferOverflow;
    }

    m_buffer.data()[m_position] = value;

    m_position += sizeof(uint8_t);

    return Error::Ok;
}

Result BufferWriter::writeU16(uint16_t value)
{
    if (remaining() < sizeof(uint16_t))
    {
        return Error::BufferOverflow;
    }

    byte* data = m_buffer.data() + m_position;

    if (m_endian == Endian::Big)
    {
        data[0] = static_cast<byte>((value >> 8) & 0xFF);
        data[1] = static_cast<byte>(value & 0xFF);
    }
    else
    {
        data[0] = static_cast<byte>(value & 0xFF);
        data[1] = static_cast<byte>((value >> 8) & 0xFF);
    }

    m_position += sizeof(uint16_t);

    return Error::Ok;
}

Result BufferWriter::writeU32(uint32_t value)
{
    if (remaining() < sizeof(uint32_t))
    {
        return Error::BufferOverflow;
    }

    byte* data = m_buffer.data() + m_position;

    if (m_endian == Endian::Big)
    {
        data[0] = static_cast<byte>((value >> 24) & 0xFF);
        data[1] = static_cast<byte>((value >> 16) & 0xFF);
        data[2] = static_cast<byte>((value >> 8) & 0xFF);
        data[3] = static_cast<byte>(value & 0xFF);
    }
    else
    {
        data[0] = static_cast<byte>(value & 0xFF);
        data[1] = static_cast<byte>((value >> 8) & 0xFF);
        data[2] = static_cast<byte>((value >> 16) & 0xFF);
        data[3] = static_cast<byte>((value >> 24) & 0xFF);
    }

    m_position += sizeof(uint32_t);

    return Error::Ok;
}

Result BufferWriter::writeU64(uint64_t value)
{
    if (remaining() < sizeof(uint64_t))
    {
        return Error::BufferOverflow;
    }

    byte* data = m_buffer.data() + m_position;

    if (m_endian == Endian::Big)
    {
        data[0] = static_cast<byte>((value >> 56) & 0xFF);
        data[1] = static_cast<byte>((value >> 48) & 0xFF);
        data[2] = static_cast<byte>((value >> 40) & 0xFF);
        data[3] = static_cast<byte>((value >> 32) & 0xFF);
        data[4] = static_cast<byte>((value >> 24) & 0xFF);
        data[5] = static_cast<byte>((value >> 16) & 0xFF);
        data[6] = static_cast<byte>((value >> 8) & 0xFF);
        data[7] = static_cast<byte>(value & 0xFF);
    }
    else
    {
        data[0] = static_cast<byte>(value & 0xFF);
        data[1] = static_cast<byte>((value >> 8) & 0xFF);
        data[2] = static_cast<byte>((value >> 16) & 0xFF);
        data[3] = static_cast<byte>((value >> 24) & 0xFF);
        data[4] = static_cast<byte>((value >> 32) & 0xFF);
        data[5] = static_cast<byte>((value >> 40) & 0xFF);
        data[6] = static_cast<byte>((value >> 48) & 0xFF);
        data[7] = static_cast<byte>((value >> 56) & 0xFF);
    }

    m_position += sizeof(uint64_t);

    return Error::Ok;
}

Result BufferWriter::writeBool(bool value)
{
    return writeU8(value ? TRUE_BYTE : FALSE_BYTE);
}

Result BufferWriter::writeFloat(float value)
{
    static_assert(
        sizeof(float) == sizeof(uint32_t),
        "float must have 32 bits"
    );

    uint32_t encodedValue = 0;

    std::memcpy(
        &encodedValue,
        &value,
        sizeof(encodedValue)
    );

    return writeU32(encodedValue);
}

Result BufferWriter::writeDouble(double value)
{
    static_assert(
        sizeof(double) == sizeof(uint64_t),
        "double must have 64 bits"
    );

    uint64_t encodedValue = 0;

    std::memcpy(
        &encodedValue,
        &value,
        sizeof(encodedValue)
    );

    return writeU64(encodedValue);
}

Result BufferWriter::writeBytes(
    const void* data,
    usize size
)
{
    if (data == nullptr && size != 0)
    {
        return Error::InvalidPointer;
    }

    if (size > remaining())
    {
        return Error::BufferOverflow;
    }

    if (size != 0)
    {
        std::memcpy(
            m_buffer.data() + m_position,
            data,
            size
        );
    }

    m_position += size;

    return Error::Ok;
}

Result BufferWriter::writeString(const char* text)
{
    if (text == nullptr)
    {
        return Error::InvalidPointer;
    }

    const usize length =
        static_cast<usize>(std::strlen(text)) + 1;

    return writeBytes(text, length);
}

//=============================================================================
// Endian
//=============================================================================

void BufferWriter::setEndian(Endian endian)
{
    m_endian = endian;
}

Endian BufferWriter::endian() const
{
    return m_endian;
}

} // namespace core
} // namespace mge