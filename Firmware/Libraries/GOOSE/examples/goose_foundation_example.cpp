#include <iostream>
#include "mge/goose/goose_message.h"

int main()
{
    using namespace mge;

    goose::Message<10> message;
    message.controlBlock().setAppId(0x1001);
    message.controlBlock().setGoCbRef("MGEIED/LLN0$GO$gcb1");
    message.controlBlock().setDataSetReference("MGEIED/LLN0$RemoteCommands");
    message.controlBlock().setGoId("MGE_COMMANDS");

    for (int i = 0; i < 10; ++i)
        message.dataset().addBoolean(false);

    std::cout << "APPID: 0x" << std::hex
              << message.controlBlock().appId() << '\n';
    std::cout << "Dataset entries: " << std::dec
              << message.dataset().size() << '\n';
    std::cout << "Message valid: "
              << (message.valid() ? "yes" : "no") << '\n';
    return message.valid() ? 0 : 1;
}
