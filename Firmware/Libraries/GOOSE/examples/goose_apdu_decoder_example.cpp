#include <iostream>

#include "mge/goose/goose_apdu_decoder.h"
#include "mge/goose/goose_apdu_encoder.h"

int main()
{
    using namespace mge;

    goose::Message<4> message;
    message.controlBlock().setAppId(0x1001);
    message.controlBlock().setDestinationMac(
        ethernet::MacAddress(0x01,0x0C,0xCD,0x01,0x00,0x01));
    message.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1");
    message.controlBlock().setDataSetReference("MGEIED/LLN0$DataSet1");
    message.controlBlock().setGoId("MGE_GOOSE_1");
    message.dataset().addBoolean(true);
    message.dataset().addInteger(42);

    core::byte encoded[512]{};
    core::byte fields[384]{};
    core::byte allData[128]{};

    core::BufferWriter writer(
        core::MutableBufferView(encoded, sizeof(encoded)));

    if (goose::ApduEncoder::encode(
            message,
            writer,
            core::MutableBufferView(fields, sizeof(fields)),
            core::MutableBufferView(allData, sizeof(allData))).failed())
        return 1;

    goose::Message<4> decoded;
    decoded.controlBlock().setAppId(0x1001);
    decoded.controlBlock().setDestinationMac(
        ethernet::MacAddress(0x01,0x0C,0xCD,0x01,0x00,0x01));

    if (goose::ApduDecoder::decode(
            core::BufferView(encoded, writer.position()),
            decoded).failed())
        return 1;

    std::cout << "gocbRef: " << decoded.controlBlock().goCbRef() << '\n';
    std::cout << "dataset entries: " << decoded.dataset().size() << '\n';
    std::cout << "boolean: " << decoded.dataset().get(0)->booleanValue() << '\n';
    std::cout << "integer: " << decoded.dataset().get(1)->integerValue() << '\n';
    return 0;
}
