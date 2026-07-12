#include "mge/ethernet/ethernet_parser.h"

namespace mge { namespace ethernet {

bool EthernetParser::decodeEtherType(core::u16 rawValue,
                                    EtherType& type) noexcept
{
    switch (rawValue)
    {
        case toValue(EtherType::IPv4):  type = EtherType::IPv4;  return true;
        case toValue(EtherType::ARP):   type = EtherType::ARP;   return true;
        case toValue(EtherType::VLAN):  type = EtherType::VLAN;  return true;
        case toValue(EtherType::IPv6):  type = EtherType::IPv6;  return true;
        case toValue(EtherType::GOOSE): type = EtherType::GOOSE; return true;
        default: return false;
    }
}

core::Result EthernetParser::parse(const core::BufferView& rawFrame,
                                   EthernetFrame& frame) noexcept
{
    if (rawFrame.data() == nullptr)
        return core::Error::InvalidPointer;

    if (rawFrame.size() < EthernetFrame::HeaderSize ||
        rawFrame.size() > EthernetFrame::MaximumFrameWithoutVlan)
        return core::Error::InvalidLength;

    const core::byte* bytes = rawFrame.data();

    const MacAddress destination(bytes);
    const MacAddress source(bytes + MacAddress::Size);

    const core::u16 rawEtherType =
        (static_cast<core::u16>(bytes[12]) << 8) |
        static_cast<core::u16>(bytes[13]);

    EtherType etherType = EtherType::IPv4;

    if (!decodeEtherType(rawEtherType, etherType))
        return core::Error::Unsupported;

    const core::usize payloadLength =
        rawFrame.size() - EthernetFrame::HeaderSize;

    frame.setDestination(destination);
    frame.setSource(source);
    frame.setEtherType(etherType);
    frame.setPayload(
        rawFrame.subview(EthernetFrame::HeaderSize, payloadLength));

    return core::Error::Ok;
}

}} // namespace mge::ethernet
