/**
 * @file asn1_encoder.h
 * @brief Codificação ASN.1 sobre a biblioteca BER.
 */
#pragma once

#include "mge/asn1/asn1_value.h"
#include "mge/ber/ber_encoder.h"

namespace mge { namespace asn1 {

class Encoder
{
public:
    explicit Encoder(ber::Encoder& encoder) noexcept
        : m_encoder(encoder) {}

    core::Result encode(const Value& value);

private:
    ber::Encoder& m_encoder;
};

}} // namespace mge::asn1
