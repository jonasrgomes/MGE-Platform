/**
 * @file mac_address.h
 * @brief Representação de um endereço MAC Ethernet.
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace mge
{
namespace ethernet
{

class MacAddress
{
public:
    static constexpr std::size_t Size = 6;

    constexpr MacAddress() noexcept
        : m_bytes{0, 0, 0, 0, 0, 0}
    {
    }

    constexpr MacAddress(
        std::uint8_t b0,
        std::uint8_t b1,
        std::uint8_t b2,
        std::uint8_t b3,
        std::uint8_t b4,
        std::uint8_t b5) noexcept
        : m_bytes{b0, b1, b2, b3, b4, b5}
    {
    }

    explicit MacAddress(const std::uint8_t bytes[Size]) noexcept;

    static constexpr MacAddress broadcast() noexcept
    {
        return MacAddress(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    }

    constexpr const std::uint8_t* data() const noexcept
    {
        return m_bytes;
    }

    constexpr std::size_t size() const noexcept
    {
        return Size;
    }

    constexpr std::uint8_t operator[](std::size_t index) const noexcept
    {
        return index < Size ? m_bytes[index] : 0;
    }

    constexpr bool isZero() const noexcept
    {
        return
            m_bytes[0] == 0 &&
            m_bytes[1] == 0 &&
            m_bytes[2] == 0 &&
            m_bytes[3] == 0 &&
            m_bytes[4] == 0 &&
            m_bytes[5] == 0;
    }

    constexpr bool isBroadcast() const noexcept
    {
        return
            m_bytes[0] == 0xFF &&
            m_bytes[1] == 0xFF &&
            m_bytes[2] == 0xFF &&
            m_bytes[3] == 0xFF &&
            m_bytes[4] == 0xFF &&
            m_bytes[5] == 0xFF;
    }

    constexpr bool isMulticast() const noexcept
    {
        return (m_bytes[0] & 0x01U) != 0;
    }

    constexpr bool isUnicast() const noexcept
    {
        return !isZero() && !isMulticast();
    }

    constexpr bool operator==(const MacAddress& other) const noexcept
    {
        return
            m_bytes[0] == other.m_bytes[0] &&
            m_bytes[1] == other.m_bytes[1] &&
            m_bytes[2] == other.m_bytes[2] &&
            m_bytes[3] == other.m_bytes[3] &&
            m_bytes[4] == other.m_bytes[4] &&
            m_bytes[5] == other.m_bytes[5];
    }

    constexpr bool operator!=(const MacAddress& other) const noexcept
    {
        return !(*this == other);
    }

    bool toString(char* destination, std::size_t capacity) const noexcept;

private:
    std::uint8_t m_bytes[Size];
};

} // namespace ethernet
} // namespace mge
