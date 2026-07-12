/**
 * @file ber_decoder.h
 * @brief Decoder BER.
 */

#pragma once

#include <cstdint>

#include "mge/ber/ber_length.h"
#include "mge/ber/ber_tag.h"

namespace mge
{
namespace ber
{

struct TlvView
{
    Tag tag{};
    core::BufferView value{};
};

class Decoder
{
public:
    explicit Decoder(core::BufferReader& reader) noexcept
        : m_reader(reader)
    {
    }

    core::Result readTag(Tag& tag);

    core::Result readLength(core::usize& length);

    core::Result readTlv(TlvView& tlv);

    static core::Result decodeBoolean(
        const TlvView& tlv,
        bool& value);

    static core::Result decodeInteger(
        const TlvView& tlv,
        std::int64_t& value);

    static core::Result decodeEnumerated(
        const TlvView& tlv,
        std::int32_t& value);

    static core::Result decodeNull(
        const TlvView& tlv);

private:
    static core::Result decodeSignedIntegerValue(
        const core::BufferView& bytes,
        std::int64_t& value);

    core::BufferReader& m_reader;
};

} // namespace ber
} // namespace mge
