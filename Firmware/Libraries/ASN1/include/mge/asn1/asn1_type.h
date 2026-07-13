/**
 * @file asn1_type.h
 * @brief Tipos ASN.1 suportados pela fundação da MGE Platform.
 */
#pragma once

#include <cstdint>

namespace mge { namespace asn1 {

enum class Type : std::uint8_t
{
    Null,
    Boolean,
    Integer,
    OctetString,
    VisibleString,
    Utf8String,
    BitString
};

}} // namespace mge::asn1
