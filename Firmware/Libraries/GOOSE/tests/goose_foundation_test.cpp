#include <cassert>
#include <cstring>
#include <iostream>

#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"
#include "mge/goose/goose_message.h"

int main()
{
    using namespace mge;

    {
        goose::Header header;
        header.setAppId(0x1001);
        header.setLength(32);

        core::byte bytes[8]{};
        core::BufferWriter writer(core::MutableBufferView(bytes, sizeof(bytes)));
        assert(header.encode(writer).ok());
        assert(writer.position() == 8);
        assert(bytes[0] == 0x10 && bytes[1] == 0x01);
        assert(bytes[2] == 0x00 && bytes[3] == 0x20);

        core::BufferReader reader(core::BufferView(bytes, sizeof(bytes)));
        goose::Header decoded;
        assert(goose::Header::decode(reader, decoded).ok());
        assert(decoded.appId() == 0x1001);
        assert(decoded.length() == 32);
    }

    {
        goose::ControlBlock control;
        assert(control.setAppId(0x1001).ok());
        assert(control.setVlan(1, 4).ok());
        assert(control.setGoCbRef("MGEIED/LLN0$GO$gcb1").ok());
        assert(control.setDataSetReference("MGEIED/LLN0$Status").ok());
        assert(control.setGoId("MGE_STATUS").ok());
        assert(control.valid());
        assert(std::strcmp(control.goId(), "MGE_STATUS") == 0);
        assert(control.destinationMac().isMulticast());
    }

    {
        goose::Dataset<4> dataset;
        assert(dataset.addBoolean(true).ok());
        assert(dataset.addInteger(125).ok());
        assert(dataset.size() == 2);
        assert(dataset.get(0)->booleanValue());
        assert(dataset.get(1)->integerValue() == 125);
    }

    {
        goose::Message<4> message;
        assert(message.controlBlock().setAppId(0x1001).ok());
        assert(message.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1").ok());
        assert(message.controlBlock().setDataSetReference("MGEIED/LLN0$Status").ok());
        assert(message.controlBlock().setGoId("MGE_STATUS").ok());
        assert(message.dataset().addBoolean(false).ok());
        assert(message.valid());

        assert(message.stateNumber() == 1);
        assert(message.sequenceNumber() == 0);
        message.markRetransmission();
        assert(message.sequenceNumber() == 1);
        message.markStateChange();
        assert(message.stateNumber() == 2);
        assert(message.sequenceNumber() == 0);
    }

    std::cout << "All GOOSE Foundation tests passed.\n";
    return 0;
}
