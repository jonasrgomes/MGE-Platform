#include <cassert>
#include <iostream>

#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"
#include "mge/ethernet/ethernet_parser.h"
#include "mge/goose/goose_apdu_decoder.h"
#include "mge/goose/goose_frame_encoder.h"

int main()
{
    using namespace mge;

    goose::Message<4> message;
    assert(message.controlBlock().setAppId(0x1001).ok());
    assert(message.controlBlock().setDestinationMac(
        ethernet::MacAddress(0x01,0x0C,0xCD,0x01,0x00,0x01)).ok());
    assert(message.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1").ok());
    assert(message.controlBlock().setDataSetReference("MGEIED/LLN0$DS1").ok());
    assert(message.controlBlock().setGoId("MGE_GO1").ok());
    assert(message.controlBlock().setTimeAllowedToLiveMs(1000).ok());
    assert(message.controlBlock().setConfigurationRevision(1).ok());

    const core::byte timeBytes[goose::TimestampSize] =
        {0x67,0xA1,0x23,0x45,0x80,0x00,0x00,0x00};
    message.setTimestamp(goose::Timestamp(timeBytes));
    message.setStateNumber(7);
    message.setSequenceNumber(3);
    assert(message.dataset().addBoolean(true).ok());
    assert(message.dataset().addInteger(125).ok());

    const ethernet::MacAddress source(
        0x02,0x12,0x34,0x56,0x78,0x01);

    core::byte frameBytes[1514]{};
    core::byte goosePayload[1500]{};
    core::byte apdu[1400]{};
    core::byte apduContent[1300]{};
    core::byte allData[512]{};

    core::BufferWriter frameWriter(
        core::MutableBufferView(frameBytes, sizeof(frameBytes)));

    const core::Result encoded = goose::FrameEncoder::encode(
        message,
        source,
        frameWriter,
        core::MutableBufferView(goosePayload, sizeof(goosePayload)),
        core::MutableBufferView(apdu, sizeof(apdu)),
        core::MutableBufferView(apduContent, sizeof(apduContent)),
        core::MutableBufferView(allData, sizeof(allData)),
        false);

    assert(encoded.ok());
    assert(frameWriter.position() > ethernet::EthernetFrame::HeaderSize + 8);

    // Cabeçalho Ethernet.
    assert(frameBytes[0] == 0x01);
    assert(frameBytes[1] == 0x0C);
    assert(frameBytes[2] == 0xCD);
    assert(frameBytes[12] == 0x88);
    assert(frameBytes[13] == 0xB8);

    ethernet::EthernetFrame parsedFrame;
    const core::Result parsed = ethernet::EthernetParser::parse(
        core::BufferView(frameBytes, frameWriter.position()),
        parsedFrame);

    assert(parsed.ok());
    assert(parsedFrame.destination() == message.controlBlock().destinationMac());
    assert(parsedFrame.source() == source);
    assert(parsedFrame.etherType() == ethernet::EtherType::GOOSE);

    core::BufferReader gooseReader(parsedFrame.payload());
    goose::Header decodedHeader;
    assert(goose::Header::decode(gooseReader, decodedHeader).ok());
    assert(decodedHeader.appId() == 0x1001);
    assert(decodedHeader.validForPayload(gooseReader.remaining()));

    const core::BufferView decodedApdu(
        gooseReader.currentData(),
        gooseReader.remaining());

    goose::Message<4> decodedMessage;
    assert(goose::ApduDecoder::decode(decodedApdu, decodedMessage).ok());
    assert(decodedMessage.stateNumber() == 7);
    assert(decodedMessage.sequenceNumber() == 3);
    assert(decodedMessage.dataset().size() == 2);

    std::cout << "GOOSE complete frame encoder test passed.\n";
    return 0;
}
