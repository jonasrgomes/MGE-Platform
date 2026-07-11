#include "mge/ethernet/mac_address.h"

#include <cstdio>
#include <cstring>

namespace mge
{
namespace ethernet
{

MacAddress::MacAddress(const std::uint8_t bytes[Size]) noexcept
    : m_bytes{0, 0, 0, 0, 0, 0}
{
    if (bytes != nullptr)
    {
        std::memcpy(m_bytes, bytes, Size);
    }
}

bool MacAddress::toString(
    char* destination,
    std::size_t capacity) const noexcept
{
    if (destination == nullptr || capacity < 18)
    {
        return false;
    }

    const int written = std::snprintf(
        destination,
        capacity,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        static_cast<unsigned>(m_bytes[0]),
        static_cast<unsigned>(m_bytes[1]),
        static_cast<unsigned>(m_bytes[2]),
        static_cast<unsigned>(m_bytes[3]),
        static_cast<unsigned>(m_bytes[4]),
        static_cast<unsigned>(m_bytes[5]));

    return written == 17;
}

} // namespace ethernet
} // namespace mge
