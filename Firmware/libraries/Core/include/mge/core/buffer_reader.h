/******************************************************************************
 *
 * MGE Platform
 * Industrial Automation Framework
 *
 * @file        buffer_reader.h
 * @brief       Sequential binary buffer reader.
 *
 ******************************************************************************/

#pragma once

#include "buffer_view.h"
#include "result.h"

namespace mge
{
namespace core
{

class BufferReader
{
public:

    explicit BufferReader(const BufferView& buffer);

    //------------------------------------------------
    // Position
    //------------------------------------------------

    usize position() const noexcept;

    usize remaining() const noexcept;

    usize size() const noexcept;

    const byte* currentData() const noexcept;

    bool eof() const noexcept;

    void rewind() noexcept;

    Result seek(usize position);

    Result skip(usize bytes);

    //------------------------------------------------
    // Peek
    //------------------------------------------------

    Result peekU8(uint8_t& value) const;

    Result peekU16(uint16_t& value) const;

    Result peekU32(uint32_t& value) const;

    //------------------------------------------------
    // Read
    //------------------------------------------------

    Result readU8(uint8_t& value);

    Result readU16(uint16_t& value);

    Result readU32(uint32_t& value);

    Result readU64(uint64_t& value);

    Result readBool(bool& value);

    Result readFloat(float& value);

    Result readDouble(double& value);

    Result readBytes(void* destination,
                     usize size);

    Result readString(char* destination,
                      usize maxSize);

private:

    BufferView m_buffer;

    usize m_position;
};

}
}