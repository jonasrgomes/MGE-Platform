/**
 * @file ber_encoder.h
 * @brief Encoder BER.
 */
#pragma once
#include <cstdint>
#include "mge/ber/ber_length.h"
#include "mge/ber/ber_tag.h"

namespace mge { namespace ber {

class Encoder
{
public:
    explicit Encoder(core::BufferWriter& writer) noexcept : m_writer(writer) {}

    core::Result writeTag(const Tag& tag);
    core::Result writeLength(core::usize length);
    core::Result writeTlv(const Tag& tag, const void* value, core::usize length);

    core::Result writeBoolean(bool value);
    core::Result writeInteger(std::int64_t value);
    core::Result writeEnumerated(std::int32_t value);
    core::Result writeNull();

    core::Result writeOctetString(const void* data, core::usize length);
    core::Result writeVisibleString(const char* text, core::usize length);
    core::Result writeVisibleString(const char* nullTerminatedText);
    core::Result writeUtf8String(const char* text, core::usize length);
    core::Result writeUtf8String(const char* nullTerminatedText);
    core::Result writeBitString(const void* data, core::usize bitCount);

private:
    core::Result writeSignedInteger(const Tag& tag, std::int64_t value);
    core::BufferWriter& m_writer;
};

}} // namespace mge::ber
