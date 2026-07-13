/**
 * @file asn1_decoder.h
 * @brief Decodificação ASN.1 sobre a biblioteca BER.
 */
#pragma once

#include "mge/asn1/asn1_value.h"
#include "mge/ber/ber_decoder.h"

namespace mge { namespace asn1 {

class Decoder
{
public:
    static core::Result decode(const ber::TlvView& tlv,
                               Value& value);
};

}} // namespace mge::asn1
