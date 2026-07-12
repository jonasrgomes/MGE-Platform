#include "mge/ethernet/ethernet_builder.h"
namespace mge { namespace ethernet {
core::Result EthernetBuilder::build(const EthernetFrame& frame, core::BufferWriter& writer,
                                    bool padToMinimumSize)
{
    if (!frame.valid()) return core::Error::InvalidParameter;
    if (frame.serializedSize() > EthernetFrame::MaximumFrameWithoutVlan)
        return core::Error::InvalidLength;
    auto r = writer.writeBytes(frame.destination().data(), MacAddress::Size);
    if (r.failed()) return r;
    r = writer.writeBytes(frame.source().data(), MacAddress::Size);
    if (r.failed()) return r;
    writer.setEndian(core::Endian::Big);
    r = writer.writeU16(toValue(frame.etherType()));
    if (r.failed()) return r;
    r = writer.writeBytes(frame.payload().data(), frame.payload().size());
    if (r.failed()) return r;
    while (padToMinimumSize && writer.position() < EthernetFrame::MinimumFrameWithoutFcs)
    {
        r = writer.writeU8(0);
        if (r.failed()) return r;
    }
    return core::Error::Ok;
}
}}
