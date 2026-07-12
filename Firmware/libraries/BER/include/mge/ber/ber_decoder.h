/**
 * @file ber_decoder.h
 * @brief Decoder BER base.
 */

#pragma once

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

private:
    core::BufferReader& m_reader;
};

} // namespace ber
} // namespace mge
