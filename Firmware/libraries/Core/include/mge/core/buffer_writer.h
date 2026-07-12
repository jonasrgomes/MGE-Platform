/******************************************************************************
 *
 * MGE Platform
 * Industrial Automation Framework
 *
 * @file        buffer_writer.h
 * @brief       Sequential binary buffer writer.
 *
 ******************************************************************************/

#pragma once

#include "buffer_view.h"
#include "result.h"
#include "endian.h"

namespace mge
{
namespace core
{

class BufferWriter
{
public:

    explicit BufferWriter(MutableBufferView buffer);

    //------------------------------------------------
    // Position
    //------------------------------------------------

    usize position() const noexcept;

    usize remaining() const noexcept;

    usize size() const noexcept;

    bool full() const noexcept;

    void rewind() noexcept;

    Result seek(usize position);

    Result skip(usize bytes);

    //------------------------------------------------
    // Write
    //------------------------------------------------

    Result writeU8(uint8_t value);

    Result writeU16(uint16_t value);

    Result writeU32(uint32_t value);

    Result writeU64(uint64_t value);

    Result writeBool(bool value);

    Result writeFloat(float value);

    Result writeDouble(double value);

    Result writeBytes(const void* data,
                      usize size);

    Result writeString(const char* text);

    //------------------------------------------------
    // Endian
    //------------------------------------------------

    void setEndian(Endian endian);

    Endian endian() const;

private:

    MutableBufferView m_buffer;

    usize m_position;

    Endian m_endian;
};

}
}