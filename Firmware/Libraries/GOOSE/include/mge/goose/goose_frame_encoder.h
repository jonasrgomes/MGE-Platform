/**
 * @file goose_frame_encoder.h
 * @brief Montagem de um quadro Ethernet GOOSE completo sem VLAN.
 */
#pragma once

#include "mge/ethernet/ethernet_builder.h"
#include "mge/goose/goose_apdu_encoder.h"
#include "mge/goose/goose_header.h"

namespace mge { namespace goose {

class FrameEncoder
{
public:
    /**
     * @brief Codifica um quadro Ethernet GOOSE completo, sem FCS e sem VLAN.
     *
     * @param message Mensagem GOOSE a codificar.
     * @param sourceMac Endereço MAC da placa transmissora.
     * @param output Buffer final do quadro Ethernet.
     * @param goosePayloadScratch Buffer para Header GOOSE + APDU.
     * @param apduScratch Buffer para a APDU final.
     * @param apduContentScratch Buffer para o conteúdo interno da APDU.
     * @param allDataScratch Buffer para o campo allData.
     * @param padToMinimumSize Completa o quadro até 60 bytes, sem FCS.
     */
    template <core::usize Capacity>
    static core::Result encode(
        const Message<Capacity>& message,
        const ethernet::MacAddress& sourceMac,
        core::BufferWriter& output,
        core::MutableBufferView goosePayloadScratch,
        core::MutableBufferView apduScratch,
        core::MutableBufferView apduContentScratch,
        core::MutableBufferView allDataScratch,
        bool padToMinimumSize = true)
    {
        if (!message.valid())
            return core::Error::InvalidState;

        if (sourceMac.isZero() || sourceMac.isMulticast())
            return core::Error::InvalidAddress;

        core::BufferWriter apduWriter(apduScratch);
        core::Result result = ApduEncoder::encode(
            message,
            apduWriter,
            apduContentScratch,
            allDataScratch);

        if (result.failed())
            return result;

        const core::usize gooseLength =
            ProtocolHeaderSize + apduWriter.position();

        if (gooseLength > 0xFFFFU)
            return core::Error::InvalidLength;

        Header header;
        header.setAppId(message.controlBlock().appId());
        header.setLength(static_cast<core::u16>(gooseLength));
        header.setReserved1(message.header().reserved1());
        header.setReserved2(message.header().reserved2());

        core::BufferWriter gooseWriter(goosePayloadScratch);
        result = header.encode(gooseWriter);
        if (result.failed()) return result;

        result = gooseWriter.writeBytes(
            apduScratch.data(),
            apduWriter.position());
        if (result.failed()) return result;

        const ethernet::EthernetFrame frame(
            message.controlBlock().destinationMac(),
            sourceMac,
            ethernet::EtherType::GOOSE,
            core::BufferView(
                goosePayloadScratch.data(),
                gooseWriter.position()));

        return ethernet::EthernetBuilder::build(
            frame,
            output,
            padToMinimumSize);
    }
};

}} // namespace mge::goose
