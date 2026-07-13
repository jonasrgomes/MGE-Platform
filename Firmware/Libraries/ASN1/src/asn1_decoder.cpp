#include "mge/asn1/asn1_decoder.h"

namespace mge { namespace asn1 {

core::Result Decoder::decode(const ber::TlvView& tlv,
                             Value& value)
{
    if (tlv.tag == ber::UniversalTag::Null)
    {
        const core::Result result = ber::Decoder::decodeNull(tlv);
        if (result.failed()) return result;
        value = Value::null();
        return core::Error::Ok;
    }

    if (tlv.tag == ber::UniversalTag::Boolean)
    {
        bool decoded = false;
        const core::Result result = ber::Decoder::decodeBoolean(tlv, decoded);
        if (result.failed()) return result;
        value = Value::boolean(decoded);
        return core::Error::Ok;
    }

    if (tlv.tag == ber::UniversalTag::Integer)
    {
        std::int64_t decoded = 0;
        const core::Result result = ber::Decoder::decodeInteger(tlv, decoded);
        if (result.failed()) return result;
        value = Value::integer(decoded);
        return core::Error::Ok;
    }

    core::BufferView bytes;

    if (tlv.tag == ber::UniversalTag::OctetString)
    {
        const core::Result result = ber::Decoder::decodeOctetString(tlv, bytes);
        if (result.failed()) return result;
        value = Value::octetString(bytes);
        return core::Error::Ok;
    }

    if (tlv.tag == ber::UniversalTag::VisibleString)
    {
        const core::Result result = ber::Decoder::decodeVisibleString(tlv, bytes);
        if (result.failed()) return result;
        value = Value::visibleString(bytes);
        return core::Error::Ok;
    }

    if (tlv.tag == ber::UniversalTag::Utf8String)
    {
        const core::Result result = ber::Decoder::decodeUtf8String(tlv, bytes);
        if (result.failed()) return result;
        value = Value::utf8String(bytes);
        return core::Error::Ok;
    }

    if (tlv.tag == ber::UniversalTag::BitString)
    {
        ber::BitStringView decoded;
        const core::Result result = ber::Decoder::decodeBitString(tlv, decoded);
        if (result.failed()) return result;
        value = Value::bitString(decoded.bytes, decoded.bitCount);
        return core::Error::Ok;
    }

    return core::Error::Unsupported;
}

}} // namespace mge::asn1
