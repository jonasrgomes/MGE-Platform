#pragma once
#include "mge/core/buffer_view.h"
#include "mge/core/result.h"
#include "mge/ethernet/ethernet_frame.h"

namespace mge { namespace ethernet {

class EthernetParser
{
public:
    static core::Result parse(const core::BufferView& rawFrame,
                              EthernetFrame& frame) noexcept;

private:
    static bool decodeEtherType(core::u16 rawValue,
                                EtherType& type) noexcept;
};

}} // namespace mge::ethernet
