/******************************************************************************
 *
 * MGE Platform
 * Industrial Automation Framework
 *
 ******************************************************************************/

#pragma once

#include <cstdint>

#include "types.h"
#include "result.h"

namespace mge
{
namespace core
{

enum class Endian
{
    Big,
    Little
};

class Buffer
{
public:

    //--------------------------------------------------
    // Construtores
    //--------------------------------------------------

    Buffer(uint8_t* data,
           uint32_t capacity);

    //--------------------------------------------------
    // Escrita
    //--------------------------------------------------

    Result writeU8(uint8_t value);

    Result writeU16(uint16_t value);

    Result writeU32(uint32_t value);

    Result writeU64(uint64_t value);

    Result writeBool(bool value);

    Result writeFloat(float value);

    Result writeDouble(double value);

    Result writeBytes(const void* data,
                      uint32_t size);

    Result writeString(const char* text);

    //--------------------------------------------------
    // Leitura
    //--------------------------------------------------

    Result readU8(uint8_t& value);

    Result readU16(uint16_t& value);

    Result readU32(uint32_t& value);

    Result readU64(uint64_t& value);

    Result readBool(bool& value);

    Result readFloat(float& value);

    Result readDouble(double& value);

    Result readBytes(void* data,
                     uint32_t size);

    //--------------------------------------------------
    // Controle
    //--------------------------------------------------

    void clear();

    void rewind();

    void resetRead();

    void resetWrite();

    void setReadPosition(uint32_t position);

    void setWritePosition(uint32_t position);

    //--------------------------------------------------
    // Informações
    //--------------------------------------------------

    uint32_t size() const;

    uint32_t capacity() const;

    uint32_t remaining() const;

    uint32_t readPosition() const;

    uint32_t writePosition() const;

    bool empty() const;

    bool full() const;

    //--------------------------------------------------
    // Endian
    //--------------------------------------------------

    void setEndian(Endian endian);

    Endian endian() const;

    //--------------------------------------------------
    // Dados
    //--------------------------------------------------

    uint8_t* data();

    const uint8_t* data() const;

private:

    uint8_t* m_data;

    uint32_t m_capacity;

    uint32_t m_readPosition;

    uint32_t m_writePosition;

    Endian m_endian;
};

}
}