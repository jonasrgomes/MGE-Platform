/**
 * @file ber_tag.h
 * @brief Tipos de tag BER básicos.
 */

#pragma once

#include <cstdint>

namespace mge
{
namespace ber
{

enum class TagClass : std::uint8_t
{
    Universal       = 0x00,
    Application     = 0x40,
    ContextSpecific = 0x80,
    Private         = 0xC0
};

enum class PcBit : std::uint8_t
{
    Primitive   = 0x00,
    Constructed = 0x20
};

struct Tag
{
    TagClass tagClass = TagClass::Universal;
    PcBit pc = PcBit::Primitive;
    std::uint32_t number = 0;

    constexpr bool isShortForm() const noexcept
    {
        return number < 31;
    }

    constexpr std::uint8_t firstOctet() const noexcept
    {
        return static_cast<std::uint8_t>(
            static_cast<std::uint8_t>(tagClass) |
            static_cast<std::uint8_t>(pc) |
            (isShortForm() ? static_cast<std::uint8_t>(number) : 0x1F));
    }
};

namespace UniversalTag
{
constexpr Tag Boolean{TagClass::Universal, PcBit::Primitive, 1};
constexpr Tag Integer{TagClass::Universal, PcBit::Primitive, 2};
constexpr Tag BitString{TagClass::Universal, PcBit::Primitive, 3};
constexpr Tag OctetString{TagClass::Universal, PcBit::Primitive, 4};
constexpr Tag Null{TagClass::Universal, PcBit::Primitive, 5};
constexpr Tag ObjectIdentifier{TagClass::Universal, PcBit::Primitive, 6};
constexpr Tag Sequence{TagClass::Universal, PcBit::Constructed, 16};
constexpr Tag Set{TagClass::Universal, PcBit::Constructed, 17};
constexpr Tag Utf8String{TagClass::Universal, PcBit::Primitive, 12};
constexpr Tag VisibleString{TagClass::Universal, PcBit::Primitive, 26};
}

} // namespace ber
} // namespace mge
