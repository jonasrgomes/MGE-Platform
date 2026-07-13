#include <iomanip>
#include <iostream>

#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"
#include "mge/goose/goose_frame_encoder.h"

int main()
{
    using namespace mge;

    goose::Message<10> message;
    message.controlBlock().setAppId(0x1001);
    message.controlBlock().setDestinationMac(
        ethernet::MacAddress(0x01,0x0C,0xCD,0x01,0x00,0x01));
    message.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1");
    message.controlBlock().setDataSetReference("MGEIED/LLN0$RemoteCommands");
    message.controlBlock().setGoId("MGE_COMMANDS");

    for (int i = 0; i < 10; ++i)
        message.dataset().addBoolean(i == 0);

    core::byte frame[1514]{};
    core::byte goosePayload[1500]{};
    core::byte apdu[1400]{};
    core::byte apduContent[1300]{};
    core::byte allData[512]{};

    core::BufferWriter output(
        core::MutableBufferView(frame, sizeof(frame)));

    const core::Result result = goose::FrameEncoder::encode(
        message,
        ethernet::MacAddress(0x02,0x12,0x34,0x56,0x78,0x01),
        output,
        core::MutableBufferView(goosePayload, sizeof(goosePayload)),
        core::MutableBufferView(apdu, sizeof(apdu)),
        core::MutableBufferView(apduContent, sizeof(apduContent)),
        core::MutableBufferView(allData, sizeof(allData)),
        true);

    if (result.failed())
    {
        std::cerr << "Falha ao gerar o quadro GOOSE.\n";
        return 1;
    }

    std::cout << "Quadro GOOSE: " << output.position() << " bytes\n";

    for (core::usize i = 0; i < output.position(); ++i)
    {
        std::cout << std::hex << std::uppercase
                  << std::setw(2) << std::setfill('0')
                  << static_cast<unsigned>(frame[i]) << ' ';
    }

    std::cout << '\n';
    return 0;
}
