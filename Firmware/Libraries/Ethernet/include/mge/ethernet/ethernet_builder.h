#pragma once
#include "mge/core/buffer_writer.h"
#include "mge/ethernet/ethernet_frame.h"
namespace mge { namespace ethernet {
class EthernetBuilder
{
public:
    static core::Result build(const EthernetFrame& frame, core::BufferWriter& writer,
                              bool padToMinimumSize = true);
};
}}
