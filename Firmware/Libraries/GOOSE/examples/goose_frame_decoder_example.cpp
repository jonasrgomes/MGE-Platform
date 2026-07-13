#include <iostream>

#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"
#include "mge/goose/goose_frame_decoder.h"
#include "mge/goose/goose_frame_encoder.h"

int main()
{
    using namespace mge;

    goose::Message<4> tx;
    tx.controlBlock().setAppId(0x1001);
    tx.controlBlock().setDestinationMac(
        ethernet::MacAddress(0x01,0x0C,0xCD,0x01,0x00,0x01));
    tx.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1");
    tx.controlBlock().setDataSetReference("MGEIED/LLN0$dataset1");
    tx.controlBlock().setGoId("MGE-GOOSE");
    tx.dataset().addBoolean(true);

    core::byte frame[512]{};
    core::byte goosePayload[384]{};
    core::byte apdu[320]{};
    core::byte apduContent[256]{};
    core::byte allData[128]{};

    core::BufferWriter output(
        core::MutableBufferView(frame, sizeof(frame)));

    const core::Result encoded = goose::FrameEncoder::encode(
        tx,
        ethernet::MacAddress(0x02,0x12,0x34,0x56,0x78,0x01),
        output,
        core::MutableBufferView(goosePayload, sizeof(goosePayload)),
        core::MutableBufferView(apdu, sizeof(apdu)),
        core::MutableBufferView(apduContent, sizeof(apduContent)),
        core::MutableBufferView(allData, sizeof(allData)),
        true);

    if (encoded.failed())
        return 1;

    goose::Message<4> rx;
    goose::FrameMetadata metadata;

    const core::Result decoded = goose::FrameDecoder::decode(
        core::BufferView(frame, output.position()),
        rx,
        metadata);

    if (decoded.failed())
        return 1;

    char sourceText[18]{};
    metadata.sourceMac.toString(sourceText, sizeof(sourceText));

    std::cout << "Source MAC : " << sourceText << '\n';
    std::cout << "APPID      : 0x" << std::hex << metadata.header.appId() << '\n';
    std::cout << "GOOSE bytes: " << std::dec << metadata.goosePayloadSize << '\n';
    std::cout << "Padding    : " << metadata.ethernetPaddingSize << '\n';
    std::cout << "Dataset    : " << rx.dataset().size() << " entries\n";

    return 0;
}
