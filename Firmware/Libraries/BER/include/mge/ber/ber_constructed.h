/**
 * @file ber_constructed.h
 * @brief Helpers for constructed BER values and object identifiers.
 */
#pragma once

#include "mge/core/buffer_view.h"
#include "mge/ber/ber_tag.h"

namespace mge { namespace ber {

constexpr Tag contextSpecificTag(
    std::uint32_t number,
    PcBit pc = PcBit::Primitive) noexcept
{
    return Tag{TagClass::ContextSpecific, pc, number};
}

constexpr Tag applicationTag(
    std::uint32_t number,
    PcBit pc = PcBit::Primitive) noexcept
{
    return Tag{TagClass::Application, pc, number};
}

}} // namespace mge::ber
