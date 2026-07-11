/**
 * @file ether_type.h
 * @brief EtherTypes conhecidos pela MGE Platform.
 */

#pragma once

#include <cstdint>

namespace mge
{
namespace ethernet
{

enum class EtherType : std::uint16_t
{
    IPv4  = 0x0800,
    ARP   = 0x0806,
    VLAN  = 0x8100,
    IPv6  = 0x86DD,
    GOOSE = 0x88B8
};

constexpr std::uint16_t toValue(EtherType type) noexcept
{
    return static_cast<std::uint16_t>(type);
}

} // namespace ethernet
} // namespace mge
