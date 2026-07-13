#include <cassert>
#include <iostream>
#include "mge/ber/ber_decoder.h"
#include "mge/ber/ber_encoder.h"
#include "mge/core/buffer_reader.h"
#include "mge/core/buffer_writer.h"

int main()
{
    using namespace mge;

    core::byte inner[32]{};
    core::BufferWriter innerWriter(core::MutableBufferView(inner, sizeof(inner)));
    ber::Encoder innerEncoder(innerWriter);
    assert(innerEncoder.writeInteger(7).ok());
    assert(innerEncoder.writeBoolean(true).ok());

    core::byte sequenceBytes[64]{};
    core::BufferWriter sequenceWriter(core::MutableBufferView(sequenceBytes, sizeof(sequenceBytes)));
    ber::Encoder sequenceEncoder(sequenceWriter);
    assert(sequenceEncoder.writeSequence(core::BufferView(inner, innerWriter.position())).ok());
    assert(sequenceBytes[0] == 0x30);

    core::BufferReader sequenceReader(core::BufferView(sequenceBytes, sequenceWriter.position()));
    ber::Decoder sequenceDecoder(sequenceReader);
    ber::TlvView sequenceTlv;
    assert(sequenceDecoder.readTlv(sequenceTlv).ok());
    core::BufferView content;
    assert(ber::Decoder::decodeConstructed(sequenceTlv, ber::UniversalTag::Sequence, content).ok());
    assert(content.size() == innerWriter.position());

    core::byte oidBytes[64]{};
    core::BufferWriter oidWriter(core::MutableBufferView(oidBytes, sizeof(oidBytes)));
    ber::Encoder oidEncoder(oidWriter);
    const std::uint32_t oid[] = {1, 3, 6, 1, 4, 1, 55555, 1};
    assert(oidEncoder.writeObjectIdentifier(oid, 8).ok());
    assert(oidBytes[0] == 0x06);

    core::BufferReader oidReader(core::BufferView(oidBytes, oidWriter.position()));
    ber::Decoder oidDecoder(oidReader);
    ber::TlvView oidTlv;
    assert(oidDecoder.readTlv(oidTlv).ok());
    std::uint32_t decoded[16]{};
    core::usize decodedCount = 0;
    assert(ber::Decoder::decodeObjectIdentifier(oidTlv, decoded, 16, decodedCount).ok());
    assert(decodedCount == 8);
    for (core::usize i = 0; i < decodedCount; ++i) assert(decoded[i] == oid[i]);

    core::byte contextBytes[32]{};
    core::BufferWriter contextWriter(core::MutableBufferView(contextBytes, sizeof(contextBytes)));
    ber::Encoder contextEncoder(contextWriter);
    const core::byte raw[] = {0xAA, 0xBB};
    assert(contextEncoder.writeContextSpecific(3, ber::PcBit::Primitive,
            core::BufferView(raw, sizeof(raw))).ok());
    assert(contextBytes[0] == 0x83);

    std::cout << "All BER constructed and OID tests passed.\n";
    return 0;
}
