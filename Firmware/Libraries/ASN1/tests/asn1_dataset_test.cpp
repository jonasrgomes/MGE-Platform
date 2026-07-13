#include <cassert>
#include <cstring>
#include <iostream>

#include "mge/asn1/asn1_dataset.h"
#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_view.h"
#include "mge/core/buffer_writer.h"

int main()
{
    using namespace mge;

    const core::byte text[] = {'M','G','E'};
    const core::byte octets[] = {0x10,0x20,0x30};
    const core::byte bits[] = {0xA0};

    asn1::Dataset<8> dataset;
    assert(dataset.empty());
    assert(dataset.addBoolean(true).ok());
    assert(dataset.addInteger(1500).ok());
    assert(dataset.addVisibleString(core::BufferView(text, sizeof(text))).ok());
    assert(dataset.addOctetString(core::BufferView(octets, sizeof(octets))).ok());
    assert(dataset.addBitString(core::BufferView(bits, sizeof(bits)), 3).ok());
    assert(dataset.size() == 5);
    assert(!dataset.full());
    assert(dataset.get(99) == nullptr);

    core::byte encoded[128]{};
    core::BufferWriter writer(core::MutableBufferView(encoded, sizeof(encoded)));
    ber::Encoder berEncoder(writer);
    asn1::Encoder asnEncoder(berEncoder);

    assert(asn1::DatasetEncoder::encode(dataset, asnEncoder).ok());
    assert(writer.position() > 0);

    core::BufferReader reader(core::BufferView(encoded, writer.position()));
    ber::Decoder berDecoder(reader);
    asn1::Dataset<8> decoded;

    assert(asn1::DatasetDecoder::decode(berDecoder, dataset.size(), decoded).ok());
    assert(reader.eof());
    assert(decoded.size() == dataset.size());
    assert(decoded[0].is(asn1::Type::Boolean));
    assert(decoded[0].booleanValue());
    assert(decoded[1].integerValue() == 1500);
    assert(decoded[2].bytes().size() == 3);
    assert(std::memcmp(decoded[2].bytes().data(), text, 3) == 0);
    assert(decoded[3].bytes().size() == 3);
    assert(decoded[4].bitCount() == 3);

    asn1::Dataset<1> limited;
    assert(limited.addNull().ok());
    assert(limited.addBoolean(false).failed());

    std::cout << "All ASN.1 Dataset tests passed.\n";
    return 0;
}
