/**
 * @file ber_length.h
 * @brief Codificação e decodificação de comprimentos BER.
 */

#pragma once

#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_writer.h"

namespace mge
{
namespace ber
{

class Length
{
public:
    static core::Result encode(
        core::usize value,
        core::BufferWriter& writer);

    static core::Result decode(
        core::BufferReader& reader,
        core::usize& value);

    static constexpr core::usize encodedSize(
        core::usize value) noexcept
    {
        return value < 128 ? 1 :
               value <= 0xFF ? 2 :
               value <= 0xFFFF ? 3 :
               value <= 0xFFFFFF ? 4 : 5;
    }
};

} // namespace ber
} // namespace mge
