#include <iomanip>
#include <iostream>

#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"
#include "mge/goose/goose_apdu_encoder.h"

int main()
{
    using namespace mge;

    goose::Message<10> message;
    message.controlBlock().setAppId(0x1001);
    message.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1");
    message.controlBlock().setDataSetReference("MGEIED/LLN0$RemoteCommands");
    message.controlBlock().setGoId("MGE_COMMANDS");

    for (int i = 0; i < 10; ++i)
        message.dataset().addBoolean(i == 0);

    core::byte apdu[1024]{};
    core::byte fields[900]{};
    core::byte allData[512]{};
    core::BufferWriter writer(core::MutableBufferView(apdu, sizeof(apdu)));

    const auto result = goose::ApduEncoder::encode(
        message,
        writer,
        core::MutableBufferView(fields, sizeof(fields)),
        core::MutableBufferView(allData, sizeof(allData)));

    if (result.failed())
    {
        std::cerr << "Falha ao codificar APDU GOOSE.\n";
        return 1;
    }

    std::cout << "APDU GOOSE: " << writer.position() << " bytes\n";
    for (core::usize i = 0; i < writer.position(); ++i)
        std::cout << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<unsigned>(apdu[i]) << ' ';
    std::cout << '\n';
    return 0;
}
