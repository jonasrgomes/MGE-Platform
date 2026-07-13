/**
 * @file ber_decoder.h
 * @brief Decoder BER.
 */
#pragma once
#include <cstdint>
#include "mge/ber/ber_constructed.h"
#include "mge/ber/ber_length.h"
#include "mge/ber/ber_string_types.h"
#include "mge/ber/ber_tag.h"

namespace mge { namespace ber {

struct TlvView { Tag tag{}; core::BufferView value{}; };

class Decoder
{
public:
    explicit Decoder(core::BufferReader& reader) noexcept : m_reader(reader) {}

    core::Result readTag(Tag& tag);
    core::Result readLength(core::usize& length);
    core::Result readTlv(TlvView& tlv);

    static core::Result decodeBoolean(const TlvView& tlv, bool& value);
    static core::Result decodeInteger(const TlvView& tlv, std::int64_t& value);
    static core::Result decodeEnumerated(const TlvView& tlv, std::int32_t& value);
    static core::Result decodeNull(const TlvView& tlv);

    static core::Result decodeOctetString(const TlvView& tlv, core::BufferView& value);
    static core::Result decodeVisibleString(const TlvView& tlv, core::BufferView& value);
    static core::Result decodeUtf8String(const TlvView& tlv, core::BufferView& value);
    static core::Result decodeBitString(const TlvView& tlv, BitStringView& value);

    static core::Result decodeConstructed(const TlvView& tlv,
                                          const Tag& expectedTag,
                                          core::BufferView& content);
    static core::Result decodeObjectIdentifier(const TlvView& tlv,
                                               std::uint32_t* arcs,
                                               core::usize capacity,
                                               core::usize& arcCount);

private:
    static core::Result decodeSignedIntegerValue(const core::BufferView& bytes,
                                                  std::int64_t& value);
    core::BufferReader& m_reader;
};

}} // namespace mge::ber
