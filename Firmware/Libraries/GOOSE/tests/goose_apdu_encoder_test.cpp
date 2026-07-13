#include <cassert>
#include <iostream>

#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"
#include "mge/goose/goose_apdu_encoder.h"

int main()
{
    using namespace mge;

    goose::Message<4> message;
    assert(message.controlBlock().setAppId(0x1001).ok());
    assert(message.controlBlock().setGoCbRef("IED1/LLN0$GO$gcb1").ok());
    assert(message.controlBlock().setDataSetReference("IED1/LLN0$DS1").ok());
    assert(message.controlBlock().setGoId("GO1").ok());
    assert(message.controlBlock().setTimeAllowedToLiveMs(1000).ok());
    assert(message.controlBlock().setConfigurationRevision(1).ok());

    const core::byte timestampBytes[goose::TimestampSize] =
        {0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00};
    message.setTimestamp(goose::Timestamp(timestampBytes));
    message.setStateNumber(1);
    message.setSequenceNumber(0);

    assert(message.dataset().addBoolean(true).ok());
    assert(message.dataset().addInteger(125).ok());

    core::byte output[512]{};
    core::byte contentScratch[480]{};
    core::byte allDataScratch[256]{};

    core::BufferWriter outputWriter(
        core::MutableBufferView(output, sizeof(output)));

    const core::Result result = goose::ApduEncoder::encode(
        message,
        outputWriter,
        core::MutableBufferView(contentScratch, sizeof(contentScratch)),
        core::MutableBufferView(allDataScratch, sizeof(allDataScratch)));

    assert(result.ok());
    assert(outputWriter.position() > 2);
    assert(output[0] == 0x61); // Application 1, constructed

    bool foundAllData = false;
    for (core::usize i = 0; i < outputWriter.position(); ++i)
    {
        if (output[i] == 0xAB)
        {
            foundAllData = true;
            assert(i + 1 < outputWriter.position());
            break;
        }
    }
    assert(foundAllData);

    // Dentro de allData: BOOLEAN [3] e INTEGER [5].
    bool foundBoolean = false;
    bool foundInteger = false;
    for (core::usize i = 0; i < outputWriter.position(); ++i)
    {
        if (output[i] == 0x83) foundBoolean = true;
        if (output[i] == 0x85) foundInteger = true;
    }
    assert(foundBoolean);
    assert(foundInteger);

    std::cout << "GOOSE APDU Encoder test passed.\n";
    return 0;
}
