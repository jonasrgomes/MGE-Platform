#include <iomanip>
#include <iostream>

#include "mge/asn1/asn1_dataset.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"

int main()
{
    using namespace mge;

    const core::byte name[] = {'I','E','D','1'};

    asn1::Dataset<8> dataset;
    dataset.addBoolean(true);
    dataset.addInteger(123);
    dataset.addVisibleString(core::BufferView(name, sizeof(name)));

    core::byte output[128]{};
    core::BufferWriter writer(core::MutableBufferView(output, sizeof(output)));
    ber::Encoder berEncoder(writer);
    asn1::Encoder encoder(berEncoder);

    const core::Result result =
        asn1::DatasetEncoder::encode(dataset, encoder);

    if (result.failed()) return 1;

    std::cout << "Dataset ASN.1: " << dataset.size() << " entradas\n";
    for (core::usize i = 0; i < writer.position(); ++i)
    {
        std::cout << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<unsigned>(output[i]) << ' ';
    }
    std::cout << '\n';
    return 0;
}
