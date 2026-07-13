/**
 * @file goose_types.h
 * @brief Tipos fundamentais da biblioteca GOOSE.
 */
#pragma once

#include "mge/core/types.h"
#include "mge/goose/goose_constants.h"

namespace mge { namespace goose {

/** Timestamp IEC 61850 de oito octetos, preservado em formato de rede. */
class Timestamp
{
public:
    constexpr Timestamp() noexcept = default;

    explicit constexpr Timestamp(const core::byte (&bytes)[TimestampSize]) noexcept
        : m_bytes{bytes[0], bytes[1], bytes[2], bytes[3],
                  bytes[4], bytes[5], bytes[6], bytes[7]}
    {
    }

    constexpr const core::byte* data() const noexcept { return m_bytes; }
    constexpr core::usize size() const noexcept { return TimestampSize; }

    constexpr core::byte operator[](core::usize index) const noexcept
    {
        return index < TimestampSize ? m_bytes[index] : 0;
    }

    constexpr bool operator==(const Timestamp& other) const noexcept
    {
        for (core::usize i = 0; i < TimestampSize; ++i)
            if (m_bytes[i] != other.m_bytes[i]) return false;
        return true;
    }

private:
    core::byte m_bytes[TimestampSize]{};
};

enum class Simulation : core::u8
{
    Disabled = 0,
    Enabled = 1
};

}} // namespace mge::goose
