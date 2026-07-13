#include "mge/goose/goose_header.h"

namespace mge { namespace goose {

core::Result Header::encode(core::BufferWriter& writer) const
{
    writer.setEndian(core::Endian::Big);

    core::Result result = writer.writeU16(m_appId);
    if (result.failed()) return result;

    result = writer.writeU16(m_length);
    if (result.failed()) return result;

    result = writer.writeU16(m_reserved1);
    if (result.failed()) return result;

    return writer.writeU16(m_reserved2);
}

core::Result Header::decode(core::BufferReader& reader, Header& header)
{
    core::u16 appId = 0;
    core::u16 length = 0;
    core::u16 reserved1 = 0;
    core::u16 reserved2 = 0;

    core::Result result = reader.readU16(appId);
    if (result.failed()) return result;

    result = reader.readU16(length);
    if (result.failed()) return result;

    result = reader.readU16(reserved1);
    if (result.failed()) return result;

    result = reader.readU16(reserved2);
    if (result.failed()) return result;

    if (appId == 0 || length < ProtocolHeaderSize)
        return core::Error::InvalidValue;

    header.setAppId(appId);
    header.setLength(length);
    header.setReserved1(reserved1);
    header.setReserved2(reserved2);
    return core::Error::Ok;
}

}} // namespace mge::goose
