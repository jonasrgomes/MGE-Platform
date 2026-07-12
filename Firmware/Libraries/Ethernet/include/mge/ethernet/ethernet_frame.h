#pragma once
#include "mge/core/buffer_view.h"
#include "mge/ethernet/ether_type.h"
#include "mge/ethernet/mac_address.h"

namespace mge { namespace ethernet {

class EthernetFrame
{
public:
    static constexpr core::usize HeaderSize = 14;
    static constexpr core::usize MinimumFrameWithoutFcs = 60;
    static constexpr core::usize MaximumFrameWithoutVlan = 1514;

    EthernetFrame() noexcept = default;
    EthernetFrame(const MacAddress& destination, const MacAddress& source,
                  EtherType etherType, const core::BufferView& payload) noexcept
        : m_destination(destination), m_source(source),
          m_etherType(etherType), m_payload(payload) {}

    const MacAddress& destination() const noexcept { return m_destination; }
    const MacAddress& source() const noexcept { return m_source; }
    EtherType etherType() const noexcept { return m_etherType; }
    const core::BufferView& payload() const noexcept { return m_payload; }
    void setDestination(const MacAddress& v) noexcept { m_destination = v; }
    void setSource(const MacAddress& v) noexcept { m_source = v; }
    void setEtherType(EtherType v) noexcept { m_etherType = v; }
    void setPayload(const core::BufferView& v) noexcept { m_payload = v; }
    core::usize serializedSize() const noexcept { return HeaderSize + m_payload.size(); }
    bool valid() const noexcept { return !m_destination.isZero() && !m_source.isZero(); }

private:
    MacAddress m_destination;
    MacAddress m_source;
    EtherType m_etherType = EtherType::IPv4;
    core::BufferView m_payload;
};

}} // namespace mge::ethernet
