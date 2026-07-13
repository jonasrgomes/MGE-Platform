#include "mge/asn1/asn1_encoder.h"

namespace mge { namespace asn1 {

core::Result Encoder::encode(const Value& value)
{
    switch (value.type())
    {
        case Type::Null:
            return m_encoder.writeNull();

        case Type::Boolean:
            return m_encoder.writeBoolean(value.booleanValue());

        case Type::Integer:
            return m_encoder.writeInteger(value.integerValue());

        case Type::OctetString:
            return m_encoder.writeOctetString(
                value.bytes().data(), value.bytes().size());

        case Type::VisibleString:
            return m_encoder.writeVisibleString(
                reinterpret_cast<const char*>(value.bytes().data()),
                value.bytes().size());

        case Type::Utf8String:
            return m_encoder.writeUtf8String(
                reinterpret_cast<const char*>(value.bytes().data()),
                value.bytes().size());

        case Type::BitString:
            return m_encoder.writeBitString(
                value.bytes().data(), value.bitCount());
    }

    return core::Error::InvalidType;
}

}} // namespace mge::asn1
