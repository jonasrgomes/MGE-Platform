#include <cassert>
#include <cstring>
#include <iostream>

#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"
#include "mge/goose/goose_frame_decoder.h"
#include "mge/goose/goose_frame_encoder.h"

int main()
{
    using namespace mge;

    goose::Message<8> original;

    assert(original.controlBlock().setAppId(0x1001).ok());
    assert(original.controlBlock().setDestinationMac(
        ethernet::MacAddress(0x01,0x0C,0xCD,0x01,0x00,0x01)).ok());
    assert(original.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1").ok());
    assert(original.controlBlock().setDataSetReference("MGEIED/LLN0$dataset1").ok());
    assert(original.controlBlock().setGoId("MGE-GOOSE-1").ok());
    assert(original.controlBlock().setTimeAllowedToLiveMs(1000).ok());
    assert(original.controlBlock().setConfigurationRevision(7).ok());

    original.setStateNumber(11);
    original.setSequenceNumber(3);
    original.setTest(false);
    original.setNeedsCommissioning(false);

    const core::byte timestampBytes[goose::TimestampSize] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x80
    };
    const goose::Timestamp timestamp(timestampBytes);
    original.setTimestamp(timestamp);

    assert(original.dataset().addBoolean(true).ok());
    assert(original.dataset().addInteger(-25).ok());

    const core::byte textBytes[] = {'O','N'};
    assert(original.dataset().addVisibleString(
        core::BufferView(textBytes, sizeof(textBytes))).ok());

    core::byte frameMemory[512]{};
    core::byte goosePayloadMemory[384]{};
    core::byte apduMemory[320]{};
    core::byte apduContentMemory[256]{};
    core::byte allDataMemory[128]{};

    core::BufferWriter frameWriter(
        core::MutableBufferView(frameMemory, sizeof(frameMemory)));

    const ethernet::MacAddress sourceMac(
        0x02,0x12,0x34,0x56,0x78,0x01);

    core::Result result = goose::FrameEncoder::encode(
        original,
        sourceMac,
        frameWriter,
        core::MutableBufferView(goosePayloadMemory, sizeof(goosePayloadMemory)),
        core::MutableBufferView(apduMemory, sizeof(apduMemory)),
        core::MutableBufferView(apduContentMemory, sizeof(apduContentMemory)),
        core::MutableBufferView(allDataMemory, sizeof(allDataMemory)),
        true);

    assert(result.ok());
    assert(frameWriter.position() >= ethernet::EthernetFrame::MinimumFrameWithoutFcs);

    goose::Message<8> decoded;
    goose::FrameMetadata metadata;

    result = goose::FrameDecoder::decode(
        core::BufferView(frameMemory, frameWriter.position()),
        decoded,
        metadata);

    assert(result.ok());
    assert(metadata.sourceMac == sourceMac);
    assert(metadata.destinationMac == original.controlBlock().destinationMac());
    assert(metadata.header.appId() == 0x1001);
    assert(metadata.goosePayloadSize == metadata.header.length());
    assert(metadata.receivedFrameSize == frameWriter.position());

    assert(decoded.header().appId() == 0x1001);
    assert(decoded.header().length() == metadata.header.length());
    assert(decoded.controlBlock().appId() == 0x1001);
    assert(decoded.controlBlock().destinationMac() == original.controlBlock().destinationMac());
    assert(std::strcmp(decoded.controlBlock().goCbRef(), "MGEIED/LLN0$GO$gcb1") == 0);
    assert(std::strcmp(decoded.controlBlock().dataSetReference(), "MGEIED/LLN0$dataset1") == 0);
    assert(std::strcmp(decoded.controlBlock().goId(), "MGE-GOOSE-1") == 0);
    assert(decoded.controlBlock().timeAllowedToLiveMs() == 1000);
    assert(decoded.controlBlock().configurationRevision() == 7);
    assert(decoded.stateNumber() == 11);
    assert(decoded.sequenceNumber() == 3);
    assert(decoded.timestamp() == timestamp);
    assert(decoded.dataset().size() == 3);
    assert(decoded.dataset()[0].booleanValue());
    assert(decoded.dataset()[1].integerValue() == -25);
    assert(decoded.dataset()[2].bytes().size() == 2);
    assert(decoded.dataset()[2].bytes()[0] == 'O');
    assert(decoded.dataset()[2].bytes()[1] == 'N');

    // EtherType incorreto deve ser rejeitado.
    core::byte invalidEtherType[64]{};
    std::memcpy(invalidEtherType, frameMemory, 64);
    invalidEtherType[12] = 0x08;
    invalidEtherType[13] = 0x00;

    goose::Message<8> invalidMessage;
    goose::FrameMetadata invalidMetadata;
    result = goose::FrameDecoder::decode(
        core::BufferView(invalidEtherType, sizeof(invalidEtherType)),
        invalidMessage,
        invalidMetadata);

    assert(result.failed());
    assert(result.error == core::Error::InvalidType);

    // Length maior que o payload recebido deve ser rejeitado.
    core::byte invalidLength[512]{};
    std::memcpy(invalidLength, frameMemory, frameWriter.position());
    invalidLength[16] = 0x7F;
    invalidLength[17] = 0xFF;

    result = goose::FrameDecoder::decode(
        core::BufferView(invalidLength, frameWriter.position()),
        invalidMessage,
        invalidMetadata);

    assert(result.failed());
    assert(result.error == core::Error::BufferUnderflow);

    std::cout << "All GOOSE FrameDecoder tests passed.\n";
    return 0;
}
