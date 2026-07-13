/**
 * @file goose_frame_decoder.h
 * @brief Decodificação de um quadro Ethernet GOOSE completo sem VLAN.
 */
#pragma once

#include "mge/ethernet/ethernet_parser.h"
#include "mge/goose/goose_apdu_decoder.h"
#include "mge/goose/goose_header.h"

namespace mge { namespace goose {

/**
 * @brief Metadados extraídos do quadro Ethernet recebido.
 */
struct FrameMetadata
{
    ethernet::MacAddress destinationMac{};
    ethernet::MacAddress sourceMac{};
    Header header{};
    core::usize receivedFrameSize = 0;
    core::usize goosePayloadSize = 0;
    core::usize ethernetPaddingSize = 0;
};

class FrameDecoder
{
public:
    /**
     * @brief Decodifica um quadro Ethernet GOOSE completo, sem VLAN e sem FCS.
     *
     * O campo Length do cabeçalho GOOSE define o fim real da mensagem. Bytes
     * adicionais usados como padding Ethernet são ignorados.
     */
    template <core::usize Capacity>
    static core::Result decode(
        const core::BufferView& rawEthernetFrame,
        Message<Capacity>& message,
        FrameMetadata& metadata)
    {
        ethernet::EthernetFrame ethernetFrame;

        core::Result result = ethernet::EthernetParser::parse(
            rawEthernetFrame,
            ethernetFrame);

        if (result.failed())
            return result;

        if (ethernetFrame.etherType() != ethernet::EtherType::GOOSE)
            return core::Error::InvalidType;

        if (ethernetFrame.payload().size() < ProtocolHeaderSize)
            return core::Error::InvalidLength;

        core::BufferReader payloadReader(ethernetFrame.payload());
        Header header;

        result = Header::decode(payloadReader, header);
        if (result.failed())
            return result;

        if (header.appId() == 0 || header.length() < ProtocolHeaderSize)
            return core::Error::InvalidValue;

        const core::usize declaredGooseSize = header.length();

        if (declaredGooseSize > ethernetFrame.payload().size())
            return core::Error::BufferUnderflow;

        const core::usize apduSize =
            declaredGooseSize - ProtocolHeaderSize;

        if (apduSize == 0)
            return core::Error::InvalidLength;

        const core::BufferView apdu = ethernetFrame.payload().subview(
            ProtocolHeaderSize,
            apduSize);

        if (apdu.data() == nullptr)
            return core::Error::InvalidLength;

        result = ApduDecoder::decode(apdu, message);
        if (result.failed())
            return result;

        result = message.controlBlock().setAppId(header.appId());
        if (result.failed())
            return result;

        result = message.controlBlock().setDestinationMac(
            ethernetFrame.destination());
        if (result.failed())
            return result;

        message.header() = header;

        metadata.destinationMac = ethernetFrame.destination();
        metadata.sourceMac = ethernetFrame.source();
        metadata.header = header;
        metadata.receivedFrameSize = rawEthernetFrame.size();
        metadata.goosePayloadSize = declaredGooseSize;
        metadata.ethernetPaddingSize =
            ethernetFrame.payload().size() - declaredGooseSize;

        return message.valid()
            ? core::Error::Ok
            : core::Error::InvalidState;
    }
};

}} // namespace mge::goose
