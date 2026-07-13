#include <cassert>
#include <iostream>

#include "mge/goose/goose_apdu_decoder.h"
#include "mge/goose/goose_apdu_encoder.h"

int main()
{
    using namespace mge;

    goose::Message<8> source;

    assert(source.controlBlock().setAppId(0x1001).ok());
    assert(source.controlBlock().setDestinationMac(
        ethernet::MacAddress(0x01,0x0C,0xCD,0x01,0x00,0x01)).ok());
    assert(source.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1").ok());
    assert(source.controlBlock().setDataSetReference("MGEIED/LLN0$DataSet1").ok());
    assert(source.controlBlock().setGoId("MGE_GOOSE_1").ok());
    assert(source.controlBlock().setTimeAllowedToLiveMs(1000).ok());
    assert(source.controlBlock().setConfigurationRevision(7).ok());

    const core::byte timestampBytes[goose::TimestampSize] =
        {0x65,0x53,0xF1,0x00,0x80,0x00,0x00,0x0A};

    source.setTimestamp(goose::Timestamp(timestampBytes));
    source.setStateNumber(12);
    source.setSequenceNumber(34);
    source.setTest(true);
    source.setNeedsCommissioning(false);

    const core::byte octets[] = {0x10,0x20,0x30};
    const core::byte bits[] = {0xA0};
    const core::byte text[] = {'M','G','E'};

    assert(source.dataset().addBoolean(true).ok());
    assert(source.dataset().addInteger(-25).ok());
    assert(source.dataset().addOctetString(
        core::BufferView(octets, sizeof(octets))).ok());
    assert(source.dataset().addVisibleString(
        core::BufferView(text, sizeof(text))).ok());
    assert(source.dataset().addBitString(
        core::BufferView(bits, sizeof(bits)), 3).ok());

    core::byte encoded[1024]{};
    core::byte contentScratch[768]{};
    core::byte allDataScratch[256]{};

    core::BufferWriter output(
        core::MutableBufferView(encoded, sizeof(encoded)));

    auto result = goose::ApduEncoder::encode(
        source,
        output,
        core::MutableBufferView(contentScratch, sizeof(contentScratch)),
        core::MutableBufferView(allDataScratch, sizeof(allDataScratch)));

    assert(result.ok());

    goose::Message<8> decoded;
    assert(decoded.controlBlock().setAppId(0x1001).ok());
    assert(decoded.controlBlock().setDestinationMac(
        ethernet::MacAddress(0x01,0x0C,0xCD,0x01,0x00,0x01)).ok());

    result = goose::ApduDecoder::decode(
        core::BufferView(encoded, output.position()),
        decoded);

    assert(result.ok());
    assert(decoded.controlBlock().timeAllowedToLiveMs() == 1000);
    assert(decoded.controlBlock().configurationRevision() == 7);
    assert(decoded.stateNumber() == 12);
    assert(decoded.sequenceNumber() == 34);
    assert(decoded.test());
    assert(!decoded.needsCommissioning());
    assert(decoded.timestamp() == source.timestamp());
    assert(decoded.dataset().size() == 5);

    assert(decoded.dataset().get(0)->booleanValue());
    assert(decoded.dataset().get(1)->integerValue() == -25);
        assert(decoded.dataset().get(2)->bytes().size() == 3);
    assert(decoded.dataset().get(3)->bytes().size() == 3);
    assert(decoded.dataset().get(4)->bitCount() == 3);

    std::cout << "All GOOSE APDU decoder tests passed.\n";
    return 0;
}
