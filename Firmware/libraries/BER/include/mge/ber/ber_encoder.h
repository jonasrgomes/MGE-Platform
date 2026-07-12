/**
 * @file ber_encoder.h
 * @brief Encoder BER base.
 */

#pragma once

#include "mge/ber/ber_length.h"
#include "mge/ber/ber_tag.h"

namespace mge
{
namespace ber
{

class Encoder
{
public:
    explicit Encoder(core::BufferWriter& writer) noexcept
        : m_writer(writer)
    {
    }

    core::Result writeTag(const Tag& tag);

    core::Result writeLength(core::usize length);

    core::Result writeTlv(
        const Tag& tag,
        const void* value,
        core::usize length);

private:
    core::BufferWriter& m_writer;
};

} // namespace ber
} // namespace mge
