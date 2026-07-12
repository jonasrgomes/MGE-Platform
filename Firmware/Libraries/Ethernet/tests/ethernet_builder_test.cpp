#include <cassert>
#include <iostream>
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"
#include "mge/ethernet/ethernet_builder.h"
int main(){ using namespace mge; core::byte p[]={0x10,0x01,0,8,0,0,0,0};
 ethernet::EthernetFrame f(ethernet::MacAddress(1,0x0C,0xCD,1,0,2),
 ethernet::MacAddress(2,0x12,0x34,0x56,0x78,1), ethernet::EtherType::GOOSE,
 core::BufferView(p,sizeof(p))); core::byte o[128]{}; core::BufferWriter w(core::MutableBufferView(o,sizeof(o)));
 auto r=ethernet::EthernetBuilder::build(f,w,false); assert(r.ok()); assert(w.position()==22); assert(o[12]==0x88&&o[13]==0xB8);
 std::cout<<"EthernetBuilder tests passed.\n"; }
