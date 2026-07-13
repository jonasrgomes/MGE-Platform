/**
 * @file ber_string_types.h
 * @brief Visualizações BER para strings e bit strings.
 */
#pragma once

#include "mge/core/buffer_view.h"
#include "mge/core/types.h"

namespace mge { namespace ber {

struct BitStringView
{
    core::BufferView bytes{};
    core::usize bitCount = 0;
    core::u8 unusedBits = 0;

    bool bit(core::usize index, bool& value) const noexcept
    {
        if (index >= bitCount || bytes.data() == nullptr)
            return false;
        const core::usize byteIndex = index / 8;
        const core::u8 bitIndex = static_cast<core::u8>(7U - (index % 8U));
        value = (bytes[byteIndex] & static_cast<core::u8>(1U << bitIndex)) != 0;
        return true;
    }
};

}} // namespace mge::ber
