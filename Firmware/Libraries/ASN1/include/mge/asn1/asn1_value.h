/**
 * @file asn1_value.h
 * @brief Representação estática e zero-copy de valores ASN.1.
 */
#pragma once

#include <cstdint>
#include "mge/asn1/asn1_type.h"
#include "mge/core/buffer_view.h"

namespace mge { namespace asn1 {

class Value
{
public:
    constexpr Value() noexcept = default;

    static constexpr Value null() noexcept
    {
        Value value;
        value.m_type = Type::Null;
        return value;
    }

    static constexpr Value boolean(bool input) noexcept
    {
        Value value;
        value.m_type = Type::Boolean;
        value.m_boolean = input;
        return value;
    }

    static constexpr Value integer(std::int64_t input) noexcept
    {
        Value value;
        value.m_type = Type::Integer;
        value.m_integer = input;
        return value;
    }

    static constexpr Value octetString(core::BufferView input) noexcept
    {
        Value value;
        value.m_type = Type::OctetString;
        value.m_bytes = input;
        return value;
    }

    static constexpr Value visibleString(core::BufferView input) noexcept
    {
        Value value;
        value.m_type = Type::VisibleString;
        value.m_bytes = input;
        return value;
    }

    static constexpr Value utf8String(core::BufferView input) noexcept
    {
        Value value;
        value.m_type = Type::Utf8String;
        value.m_bytes = input;
        return value;
    }

    static constexpr Value bitString(core::BufferView input,
                                     core::usize bitCount) noexcept
    {
        Value value;
        value.m_type = Type::BitString;
        value.m_bytes = input;
        value.m_bitCount = bitCount;
        return value;
    }

    constexpr Type type() const noexcept { return m_type; }
    constexpr bool booleanValue() const noexcept { return m_boolean; }
    constexpr std::int64_t integerValue() const noexcept { return m_integer; }
    constexpr core::BufferView bytes() const noexcept { return m_bytes; }
    constexpr core::usize bitCount() const noexcept { return m_bitCount; }

    constexpr bool is(Type expected) const noexcept
    {
        return m_type == expected;
    }

private:
    Type m_type = Type::Null;
    bool m_boolean = false;
    std::int64_t m_integer = 0;
    core::BufferView m_bytes{};
    core::usize m_bitCount = 0;
};

}} // namespace mge::asn1
