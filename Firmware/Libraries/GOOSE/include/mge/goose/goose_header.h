/**
 * @file goose_header.h
 * @brief Cabeçalho de oito bytes do protocolo GOOSE.
 */
#pragma once

#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_writer.h"
#include "mge/goose/goose_constants.h"

namespace mge { namespace goose {

class Header
{
public:
    constexpr core::u16 appId() const noexcept { return m_appId; }
    constexpr core::u16 length() const noexcept { return m_length; }
    constexpr core::u16 reserved1() const noexcept { return m_reserved1; }
    constexpr core::u16 reserved2() const noexcept { return m_reserved2; }

    constexpr void setAppId(core::u16 value) noexcept { m_appId = value; }
    constexpr void setLength(core::u16 value) noexcept { m_length = value; }
    constexpr void setReserved1(core::u16 value) noexcept { m_reserved1 = value; }
    constexpr void setReserved2(core::u16 value) noexcept { m_reserved2 = value; }

    constexpr bool validForPayload(core::usize payloadSize) const noexcept
    {
        return m_appId != 0 &&
               m_length == ProtocolHeaderSize + payloadSize;
    }

    core::Result encode(core::BufferWriter& writer) const;
    static core::Result decode(core::BufferReader& reader, Header& header);

private:
    core::u16 m_appId = 0;
    core::u16 m_length = ProtocolHeaderSize;
    core::u16 m_reserved1 = 0;
    core::u16 m_reserved2 = 0;
};

}} // namespace mge::goose
